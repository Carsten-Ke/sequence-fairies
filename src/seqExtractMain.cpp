/**
 * @file seqExtractMain.cpp
 * @author Carsten Kemena (c.kemena@uni-muenster.de)
 * @brief 
 * @version 1.0
 * @date 2023-08-23
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

#include <cstdlib>

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "../libs/BioSeqDataLib/src/sequence/Sequence.hpp"
#include "../libs/BioSeqDataLib/src/sequence/SequenceSet.hpp"
#include "../libs/BioSeqDataLib/src/sequence/SeqSetIOManager.hpp"


#include "cmake_generated/project_version.h"

#include "seqExtract.hpp"


namespace po = boost::program_options;
namespace fs = std::filesystem;
namespace BSDL = BioSeqDataLib;

using namespace std;

int
main(int argc, char *argv[])
{
    std::string project_version(std::string(PROJECT_VERSION));
	vector<fs::path> seqFiles, indexFiles;
	fs::path inputList;
	bool index;
	po::options_description generalOpts("General options");
	generalOpts.add_options()
		("help,h", "Produces this help message")
		("in,i", po::value<vector<fs::path> >(&seqFiles)->multitoken()->value_name("FILE"), "The input file")
		("index,I", po::value<bool>(&index)->zero_tokens()->default_value(false), "Use an index file")
		("index-file,F", po::value<vector<fs::path> >(&indexFiles)->multitoken()->value_name("FILE"), "The index file")
		("input-list,l", po::value<fs::path>(&inputList)->value_name("FILE"), "File containing input files")
	;

	fs::path outFile;
	bool append;
	bool removeComments;
	bool extractOrder;
	po::options_description outputOpts("Output options");
	outputOpts.add_options()
		("out,o", po::value<fs::path>(&outFile)->value_name("FILE"), "The output file")
		("append,a", po::value<bool>(&append)->zero_tokens()->default_value(false), "Append sequences to file")
		("remove-comments,c", po::value<bool>(&removeComments)->zero_tokens()->default_value(false), "Remove comments from output")
		("extract-order", po::value<bool>(&extractOrder)->zero_tokens()->default_value(false), "Keeps the order given in the extraction line")
	;

	vector<string> extractLine;
	vector<string> lengthStrings;
	size_t numSeqs;
	string seed;
	bool remove;
	size_t column;
	//bool re;
	//string place;
	fs::path namesFile, fastaNamesFile;
	bool ignoreMissing;
    char delimiterPos, delimiterExtract;
	po::options_description extractOpts("Extract options");
	extractOpts.add_options()
		("extract,e", po::value<vector<string> >(&extractLine)->multitoken(), "The sequence(s) to extract")
        ("get-names-from-fasta,g", po::value<fs::path>(&fastaNamesFile)->value_name("FILE"), "Obtain sequences names from fasta")
		("names-file,N", po::value<fs::path>(&namesFile)->value_name("FILE"), "File with extraction lines to use")
		("delim-extract,d", po::value<char>(&delimiterExtract)->default_value('\t', "Tab"), "The delimiter to use in the extraction file")
		("column,C", po::value<size_t>(&column)->default_value(1), "The column in the extraction file to use")
		//("regex,E", po::value<bool>(&re)->default_value(false)->zero_tokens(), "Interpret extract line as regex")
		//("part,p", po::value<string>(&place)->default_value("name"), "The place to look for regex")
		("remove,r", po::value<bool>(&remove)->default_value(false)->zero_tokens(), "Remove the given sequences")
		("num-seqs,n", po::value<size_t>(&numSeqs)->default_value(0), "The number of sequences to extract")
		("seed,s", po::value<string>(&seed), "Seed for random extract function")
		("length,L", po::value<vector<string> >(&lengthStrings)->multitoken(), "Length based extraction")
		("ignore-missing,m", po::value<bool>(&ignoreMissing)->default_value(false)->zero_tokens(), "Ignore missing sequences")
	;

    bool extractSegments;
    po::options_description segOpts("Segment options");
	segOpts.add_options()
        ("extractSegments,S", po::value<bool>(&extractSegments)->zero_tokens()->default_value(false), "Extract segments of a sequence")
        ("delim-pos,D", po::value<char>(&delimiterPos)->default_value(':'), "The delimiter to use to separate name from positions")
    ;
	bool doRevComp, doTranslate;
	string translationTable;
	po::options_description modifyOpts("Modifying options");
	modifyOpts.add_options()
		("translate,t", po::value<bool>(&doTranslate)->zero_tokens()->default_value(false), "Translate into amino acid")
		("table,T", po::value<string>(&translationTable)->default_value("standard"), "The translation table to use")
		("reverse-complement,R", po::value<bool>(&doRevComp)->zero_tokens()->default_value(false), "Calculate the reverse complement")
	;


	po::options_description all("seqExtract " + project_version + " (C) 2021  Carsten Kemena\nThis program comes with ABSOLUTELY NO WARRANTY;\n\nAllowed options are displayed below");
	all.add(generalOpts).add(outputOpts).add(extractOpts).add(segOpts).add(modifyOpts);

	po::variables_map vm;

	try
	{
		po::store(po::command_line_parser(argc, argv).options(all).run(), vm);
		if (vm.count("help"))
		{
			std::cout << all << "\n";
			exit(EXIT_SUCCESS);
		}
		po::notify(vm);
		if ((!vm.count("in")) && (!vm.count("input-list")))
		{
			throw std::runtime_error("No input files are given!");
		}

	}
	catch(std::exception &e)
	{
		std::cerr << "Error parsing command line: " << e.what() << endl;
		std::cerr << "Use -h/--help for option information" << endl;
		exit(EXIT_FAILURE);
	}
    if (!fastaNamesFile.empty())
    {
        extractNamesFromFasta(fastaNamesFile, extractLine);
    }

	if (!namesFile.empty())
    {
		readNamesFile(namesFile, extractLine, delimiterExtract, column-1);
    }

	// read names File
	if (!inputList.empty())
    {
		readInputFile(inputList, seqFiles, indexFiles);
    }

	if ((!indexFiles.empty()) && (seqFiles.size() != indexFiles.size()))
	{
		cerr << "Error! If you provide indexFiles you need to provide them for every sequence file given\n";
		exit(EXIT_FAILURE);
	}


	std::ios_base::openmode mode;
	if (append)
		mode = std::ios_base::app;
	else
		mode = std::ios_base::out;

	/*short placeN;
	if (place == "name")
		placeN = 0;
	else
	{
		if (place == "comment")
			placeN = 1;
		else
			placeN = 2;
	}*/

	vector<string> nameSet;
	map<string, size_t> order;
	Targets positions;
    string delims = "";
    delims.push_back(delimiterPos);
	bool subsection = false;
    try
    {
        subsection = parseExtractionLine(extractLine, nameSet, order, positions, extractSegments, delims);
    }
    catch(std::exception &e)
    {
        cerr << "An error occured when parsing the extract arguments! Is there possibly a problem with the delimiter used for subsection extraction or was no '-' used?\n";
        exit(EXIT_FAILURE);
    }


	BSDL::SequenceSet set;
	BSDL::SeqSetIOManager seqSetIO;
	seqSetIO.loadDefaultInputStrategies();
	seqSetIO.loadDefaultOutputStrategies();
	if (!indexFiles.empty())
		index = true;
        std::map<std::string, unsigned int> seqCounter;
	try
	{
		BSDL::SequenceSet newSet;
		if (extractOrder)
		{
			newSet.resize(order.size());
		}
		size_t nFiles = seqFiles.size();
		for (size_t i=0; i<nFiles; ++i)
		{
			auto &seqFile = seqFiles[i];
			if (index)
			{
				fs::path indexF;
				if (indexFiles.empty())
				{
					indexF = seqFiles[i];
					indexF.replace_extension("sei");
				}
				else
				{
					indexF = indexFiles[i];
				}

				if (!exists(indexF))
				{
					seqSetIO.makeIndex(seqFile, indexF);
				}
				//if (!re)
				seqSetIO.read(set, seqFile, indexF, nameSet, remove);
				//else
					//set = seqSetIO.read(seqFile, indexF, extractLine[0], placeN, !remove);
			}
			else
			{
				//if (!re)
				set = seqSetIO.read(seqFile, nameSet, remove);
				//else
				//	set.read(seqFile, extractLine[0], placeN, !remove);
			}

			for (auto &seq : set)
				seqCounter[seq.name()] += 1;

			if (subsection)
			{
				size_t nSeqs=set.size();
				for (size_t i=0; i<nSeqs; ++i)
				{
					auto range = positions.equal_range(set[i].name());

					for (auto it=range.first; it!=range.second; ++it)
					{
						bool first = true;
						auto seq = set[i].seq();
						string name = set[i].name();
						string sequence;
						if (!it->second.empty())
						{
							for (const auto &elem : it->second)
							{
								size_t start = elem.first;
								sequence.append(seq.substr(start,elem.second-start+1));
								name.append((first ? delims : ",") + to_string(start+1) + "-" + to_string(elem.second+1));
								first = false;
							}
							newSet.emplace_back(BSDL::Sequence(std::move(name), std::move(sequence), "", ""));
						}
						else
							newSet.emplace_back(BSDL::Sequence(set[i].name(), set[i].seq(), "", ""));
					}
				}

			}
			else
			{
				if (numSeqs != 0)
					randExtract(set, newSet, numSeqs, seed);
				else
				{
					if (!lengthStrings.empty())
						lengthExtract(set, newSet, lengthStrings);
					else
					{
						if (extractOrder)
						{
							size_t nSeqs = set.size();
							for (size_t i=0; i<nSeqs; ++i)
							{
								size_t pos = order[set[i].name()];
								newSet[pos] = std::move(set[i]);
							}
							set.clear();
						}
						else
						{
							size_t nSeqs = set.size();
							for (size_t i = 0; i<nSeqs; ++i)
							{
								newSet.emplace_back(std::move(set[i]));
							}
								//newSet.emplace_back(std::move(set[i].name()), std::move(set[i].seq()), "", std::move(set[i].comment()));
							set.clear();
						}
					}
				}
			}

			if (doRevComp)
				revComp(newSet);
			if (doTranslate)
				translate(newSet, translationTable);

			if (i > 0)
				mode = std::ios_base::app;
		}

		if (removeComments)
		{
			for (auto &seq : newSet)
			{
				seq.comment("");
			}
		}
		
		seqSetIO.write(newSet, "fasta", outFile, mode);//		newSet.write(outFile, removeComments, mode);
	}
	catch (std::exception &e)
	{
		std::cerr << "Error! " << e.what() << "\n";
		return EXIT_FAILURE;
	}

	if ((!ignoreMissing) && (!remove))// && (!re))
	{
		bool foundProblem = false;
		for (auto &name : nameSet)
		{
			if (seqCounter.count(name) == 0)
			{
				std::cerr << "Warning! Sequence '" << name << "' not found!\n";
				foundProblem = true;
			}
		}
		if (foundProblem)
			return 1;
	}

	return EXIT_SUCCESS;
}

