/**
 * @file seqCheckMain.cpp
 * @author Carsten Kemena (c.kemena@uni-muenster.de)
 * @brief 
 * @version 1.0
 * @date 2023-08-25
 * 
 * @copyright Copyright (c) 2023
 * 
 * This file is part of BioSeqDataLib.
 * 
 * BioSeqDataLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * BioSeqDataLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with BioSeqDataLib.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <algorithm>
#include <filesystem>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <regex>

// Boost
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

// BSDL header
#include "../libs/BioSeqDataLib/src/bio/Alphabet.hpp"
#include "../libs/BioSeqDataLib/src/sequence/SequenceSet.hpp"
#include "../libs/BioSeqDataLib/src/sequence/SeqSetIOManager.hpp"
#include "../libs/BioSeqDataLib/src/utility/Output.hpp"
// other
#include "cmake_generated/project_version.h"

import seqCheck;

using namespace std;
namespace po = boost::program_options;
namespace fs = std::filesystem;
namespace BSDL = BioSeqDataLib;


int
main(int argc, char *argv[])
{
	std::string project_version(std::string(PROJECT_VERSION));
	fs::path sequenceFile;
    fs::path outFile, reportFile;
    bool checkAll, fixAndKeep, fixAndRemove;
	po::options_description general("General options");
	general.add_options()
        ("help,h", "Produces this help message")
        ("in,i", po::value<fs::path>(&sequenceFile)->required()->multitoken()->value_name("FILE"), "The sequence file")
        ("out,o", po::value<fs::path>(&outFile)->value_name("FILE"), "The fasta output file")
		("report,r", po::value<fs::path>(&reportFile)->value_name("FILE"), "The file to store the report in")
        ("check-all,A", po::value<bool>(&checkAll)->default_value(false)->zero_tokens(), "run all checks")
		("fix-and-keep", po::value<bool>(&fixAndKeep)->default_value(false)->zero_tokens(), "Fix all problems without removing a sequence")
		("fix-and-remove", po::value<bool>(&fixAndRemove)->default_value(false)->zero_tokens(), "Fix all Problems by removing sequences (except stop at ends)")
    ;
	
    bool checkStops, fixEnd;
    string stopChars;
    bool removePseudogenes;
    bool replaceStopChar;
	po::options_description stopOpts("Stop check options");
	stopOpts.add_options()
		("check-stops", po::value<bool>(&checkStops)->default_value(false)->zero_tokens(), "Checks for stop chars")
		("fix-ending", po::value<bool>(&fixEnd)->default_value(false)->zero_tokens(), "Removes all stops at the end of a sequence")
		("remove-stop-genes", po::value<bool>(&removePseudogenes)->zero_tokens()->default_value(false), "Remove all genes with stops")
        ("stop-char", po::value<string>(&stopChars)->default_value(".*"), "The stop chars to use")
        ("replace-stop", po::value<bool>(&replaceStopChar)->zero_tokens()->default_value(false), "Replace in sequence stops with ambiguous char")
	;

    bool checkAlphabet, removeAlpha, replaceAlphaChar;
    std::string alphabet;
	po::options_description alphabetOpts("Alphabet check options");
	alphabetOpts.add_options()
        ("check-alphabet", po::value<bool>(&checkAlphabet)->default_value(false)->zero_tokens(), "Check if a usual alphabet is used")
        ("set-alphabet", po::value<string>(&alphabet)->default_value("protein"), "The alphabet to use")
        ("replace-char", po::value<bool>(&replaceAlphaChar)->default_value(false)->zero_tokens(), "Replace weird chars with ambigious one")
		("remove-alpha", po::value<bool>(&removeAlpha)->default_value(false)->zero_tokens(), "Removes sequences with problematic chars")
    ;

    bool checkDuplicates, renameDuplicates, removeDuplicates;
    po::options_description duplicateOpts("Duplicate check options");
	duplicateOpts.add_options()
	    ("check-duplicates", po::value<bool>(&checkDuplicates)->default_value(false)->zero_tokens(), "Check for sequences with the same name")
        ("rename-duplicates", po::value<bool>(&renameDuplicates)->default_value(false)->zero_tokens(), "Renames the duplicates")
        ("remove-duplicates", po::value<bool>(&removeDuplicates)->default_value(false)->zero_tokens(), "Removes duplicates")
    ;

	po::options_description visibleOpts("seqCheck " + project_version + " (C) 2021-2023  Carsten Kemena\nThis program comes with ABSOLUTELY NO WARRANTY;\n\nAllowed options are displayed below.");
	visibleOpts.add(general).add(stopOpts).add(alphabetOpts).add(duplicateOpts);
	
	po::variables_map vm;
	try
	{
		po::store(po::command_line_parser(argc, argv).options(visibleOpts).run(), vm);
		if (vm.count("help"))
		{
			cout << visibleOpts<< "\n";
			return EXIT_SUCCESS;
		}
		po::notify(vm);
	}
	catch (boost::program_options::error &e)
	{
		cerr << "An error occurred parsing the command line: " << e.what() << "\n";
		cerr << "Please use -h/--help for more information.\n";
		return EXIT_FAILURE;
	}

    //////////////////////////  Option handling ////////////////////////////////

	if (fixAndKeep)
	{
		checkAll = renameDuplicates = replaceAlphaChar = fixEnd = replaceStopChar = true;
	}

	if (fixAndRemove)
	{
		checkAll = removeAlpha = removeDuplicates = fixEnd = removePseudogenes = true;
	}

	if (checkAll)
	{
		checkStops = checkAlphabet = checkDuplicates = true;
	}


	if (fixAndKeep && fixAndRemove)
	{
		std::cerr << "Error! The options fix-and-keep and fix-and-remove are mutually exclusive!\n";
		exit(1);
	}
	
	if (removePseudogenes || replaceStopChar || fixEnd)
	{
		checkStops = true;
	}

	if (renameDuplicates && removeDuplicates)
	{
		std::cerr << "Error! The options remove-duplicates and rename-duplicates are mutually exclusive!\n";
		exit(1);
	}

	if (renameDuplicates || removeDuplicates)
	{
		checkDuplicates = true;
	}

	if (replaceAlphaChar && removeAlpha)
	{
		std::cerr << "Error! The options replace-char and remove-alpha are mutually exclusive!\n";
		exit(1);
	}

	if (replaceAlphaChar || removeAlpha)
	{
		checkAlphabet = true;
	}

	if (removePseudogenes && replaceStopChar)
	{
		std::cerr << "Error! The options remove-stop-genes and replaceStop are mutually exclusive!\n";
		exit(1);
	}


	/////////////////////////////////// Program start ////////////////////////////////////

	BSDL::Output reportF(reportFile);

    BSDL::SequenceSet seqSet;
	BSDL::SeqSetIOManager seqSetIO;
	seqSetIO.loadDefaultInputStrategies();
	seqSetIO.loadDefaultOutputStrategies();

    try
	{
        seqSet = seqSetIO.read(sequenceFile);
	}
	catch(std::ios_base::failure &exception)
	{
		cout << exception.what() << endl;
		exit(EXIT_FAILURE);
	}

    std::set<size_t> seqs2delete;

	// set sequence type
	BSDL::Alphabet::AType atype;
	std::map<std::string, BSDL::Alphabet::AType> string2type = {
		{"protein", BSDL::Alphabet::AType::AminoAcid},
		{"DNA", BSDL::Alphabet::AType::DNA},
		{"RNA", BSDL::Alphabet::AType::RNA},
	};

	atype = string2type[alphabet];
	char fixChar;
	if (atype == BSDL::Alphabet::AType::AminoAcid)
	{
		fixChar = 'X';
	}
	else
	{
		fixChar = 'N';
	}
	std::map<string, int> duplicateCounter;
    
	size_t seqID = 0;
	size_t nSeqs = seqSet.size();
	std::vector<Problems> problems;
	for (auto &seq : seqSet)
    {
		Problems prob(seq.name());
		if (checkStops)
		{
			auto pos = seq.seq().find_first_of(stopChars);
			if (pos != std::string::npos)
			{
				prob.stopCodons = true;
				prob.hasProblem = true;
			}

			// check for stops at end of sequence
			auto found = stopChars.find(seq[seq.size()-1]);
			if (fixEnd)
			{
				if (found != std::string::npos)
				{
					prob.fixed = true;
				}
				while (found != std::string::npos)
				{
					seq.resize(seq.size()-1);
					found = stopChars.find(seq[seq.size()-1]);
				}
			}

			// check for stops in the middle of a sequence
			pos = seq.seq().find_first_of(stopChars);
			if (pos != std::string::npos)
			{
				if (removePseudogenes)
				{
					seqs2delete.emplace(seqID);
					prob.removed = true;
				}
				else
				{
					if (replaceStopChar)
					{
						prob.fixed = true;
						for (char c : stopChars)
						{
							std::replace(seq.begin(), seq.end(), c, fixChar);
						}
					}
				}
			}
		}

		if ((checkAlphabet) && (!prob.removed))
		{
			auto problemChars = BSDL::Alphabet::checkSeqType(seq, atype);
			// removeStopChars from problematic chars
			std::string tmp = "";
			for (auto c : problemChars)
			{
				if (stopChars.find(c) == std::string::npos)
				{
					tmp += c;
				}
			}
			problemChars = tmp;
			if (!problemChars.empty())
			{
				prob.problemChars = problemChars;
				prob.hasProblem = true;
				if (replaceAlphaChar)
				{
					prob.fixed = true;
					for (auto c : problemChars)
					{
						std::replace(seq.begin(), seq.end(), c, fixChar);
					}
				}
				else if (removeAlpha)
				{
					prob.removed = true;
					seqs2delete.emplace(seqID);
				}
			} 
		}

		if ((checkDuplicates) && (!prob.removed))
		{
			auto it = duplicateCounter.find(seq.name());
			if (it != duplicateCounter.end())
			{
				prob.duplicate = true;
				prob.hasProblem = true;
				++it->second;
				if (removeDuplicates)
				{
					prob.removed = true;
					seqs2delete.emplace(seqID);
				} 
				else if (renameDuplicates)
				{
					prob.fixed = true;
					seq.name(seq.name() + "-" + std::to_string(it->second));
				}
			}
			else
			{
				duplicateCounter.emplace(seq.name(), 1);
			}
		}
		++seqID;
		if (prob.hasProblem)
		{
			problems.emplace_back(prob);
		}
    }

	seqID=0;
	BSDL::SequenceSet newSeqSet;
	for (auto &seq : seqSet)
	{
		if (seqs2delete.find(seqID) == seqs2delete.end())
		{
			newSeqSet.emplace_back(std::move(seq));
		}
		++seqID;
	}

	if (renameDuplicates | replaceAlphaChar | fixEnd | replaceStopChar | removeAlpha | removeDuplicates | fixEnd | removePseudogenes)
	{
		seqSetIO.write(newSeqSet, "fasta", outFile);
	}
	printReport(problems, nSeqs, reportF);

}

