/**
 * @file concatenatorMain.cpp
 * @author Carsten Kemena (c.kemena@uni-muenster.de)
 * @brief Concatenator is a program to concatenate alignments
 * @version 1.0
 * @date 2023-08-11
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

// C++
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>

// Boost
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

// BSDL header
#include "../libs/BioSeqDataLib/src/sequence/SequenceSet.hpp"
#include "../libs/BioSeqDataLib/src/sequence/SeqSetIOManager.hpp"
#include "../libs/BioSeqDataLib/src/utility/Output.hpp"

#include "cmake_generated/project_version.h"

import concatenator;

namespace fs = std::filesystem;
namespace po = boost::program_options;


auto hasEnding (std::string const &fullString, std::string const &ending) -> bool {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
	}
    return false;
}

auto 
main (int argc, char *argv[]) -> int
{
	std::vector<fs::path> sequence_files;
	fs::path input_directory;
    fs::path outFile;
	std::string ending;
	bool lenient;
	std::string delimiter;
	std::vector<std::string> patterns;
	po::options_description general("General options");
	general.add_options()
		("help,h", "Produces this help message")
		("in,i", po::value<std::vector<fs::path> >(&sequence_files)->multitoken()->value_name("FILE"), "The alignment files")
		("directory,d", po::value<fs::path>(&input_directory)->value_name("DIRECTORY"), "The directory with the alignment files")
		("ending,e", po::value<std::string>(&ending)->value_name("STRING")->default_value(".fa"), "File ending to be used")
		("lenient,l", po::value<bool>(&lenient)->default_value(false)->zero_tokens(), "Allows for missing sequences which will be replaced with gaps")
		("out,o", po::value<fs::path>(&outFile)->value_name("FILE")->default_value(""), "The output file")
	;

	po::options_description nameDetection("Name detection options");
	nameDetection.add_options()
		("delimiter,D", po::value<std::string> (&delimiter)->default_value(""), "Delimiter to use, default take whole sequence name")
		("pattern,p", po::value<std::vector<std::string> >(&patterns)->multitoken(), "The pattern to use for matching")
	;
	
    std::string project_version(std::string(PROJECT_VERSION));
	po::options_description visibleOpts("concatenator " + project_version + " (C) 2023  Carsten Kemena\nThis program comes with ABSOLUTELY NO WARRANTY;\n\nAllowed options are displayed below");
	visibleOpts.add(general).add(nameDetection);

	try
	{
		po::variables_map vm;
		po::store(po::command_line_parser(argc, argv).options(visibleOpts).run(), vm);
		if (vm.count("help"))
		{
			std::cout << visibleOpts<< "\n";
			return EXIT_SUCCESS;
		}
		po::notify(vm);
	}
	catch (boost::program_options::error &e)
	{
		std::cerr << "An error occurred parsing the command line: " << e.what() << std::endl;
		std::cerr << "Please use -h/--help for more information.\n";
		return EXIT_FAILURE;
	}
	if (delimiter.empty() && patterns.empty())
	{
		delimiter.push_back('\r');
	}

	BioSeqDataLib::SeqSetIOManager seqSetIO;
	seqSetIO.loadDefaultInputStrategies();
	seqSetIO.loadDefaultOutputStrategies();

	if (!input_directory.empty())
	{
		if (!fs::is_directory(input_directory))
		{
			std::cerr << "Error! '" << input_directory.string() << "' is not a directory!" << std::endl;
			exit(EXIT_FAILURE);
		}
		for (const auto & entry : fs::directory_iterator(input_directory))
		{
			if ((!fs::is_directory(entry)) && (hasEnding(entry.path().string(), ending)))
			{
				sequence_files.emplace_back(std::move(entry.path()));
			}
		}
		if (sequence_files.empty())
		{
			std::cerr << "Error! No input files found matching your criteria!" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	if (sequence_files.empty())
	{
		std::cerr << "Error! No input files provided!" << std::endl;
		exit(EXIT_FAILURE);
	}


	if ((!patterns.empty()) && (!delimiter.empty()))
	{
		std::cerr << "Error! The pattern and the delimiter parameter are mutually exclusive!" << std::endl;
		return EXIT_FAILURE;
	}

	// check if patterns are uniq
	if (!patterns.empty())
	{
		std::set<std::string> uniq;
		for (auto &pattern : patterns)
		{
			uniq.emplace(pattern);
		}
		if (uniq.size() < patterns.size())
		{
			std::cerr << "Error! Pattern list contains duplicated entry(s)!" << std::endl;
			return EXIT_FAILURE;
		}
	}

	BioSeqDataLib::SequenceSet outSet;
    try
	{
		// initialize dataset with first names
		if (patterns.empty())
		{
			BioSeqDataLib::SequenceSet seqSet = seqSetIO.read(sequence_files.front());
			std::map<std::string, int> names = createNamesIndex(seqSet, delimiter);
			for (const auto &elem : names)
			{
				outSet.emplace_back(elem.first, "", "", "");
				outSet.file(seqSet.file());
			}
		}
		else
		{
			for (const auto &pattern : patterns)
			{
				outSet.emplace_back(pattern, "", "", "");
			}
		}

		for (const auto &fileName : sequence_files)
		{
			std::map<std::string, int> names1, names2;
        	BioSeqDataLib::SequenceSet seqSet = seqSetIO.read(fileName);
			if (patterns.empty())
    		{
       			names1 = createNamesIndex(outSet, delimiter);
       			names2 = createNamesIndex(seqSet, delimiter);
    		}
    		else
    		{
        		names1 = createNamesIndex(outSet, patterns);
        		names2 = createNamesIndex(seqSet, patterns);
    		}

			auto mapping = createMatches(names1, outSet.file(), names2, seqSet.file(), lenient);
			concatenate(outSet, seqSet, mapping);
		}
	}
	catch(std::ios_base::failure &exception)
	{
		std::cerr << exception.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	catch(BioSeqDataLib::FormatException &exception)
	{
		std::cerr << exception.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	catch(std::runtime_error &exception)
	{
		std::cerr << exception.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	try
	{
		if (outFile.empty())
		{
			seqSetIO.write(outSet, "fasta", "");
		}
		else
		{
			seqSetIO.write(outSet, "fasta", outFile);
		}
	}
	catch (std::ios_base::failure &exception)
	{
		std::cout << exception.what() << std::endl;
		exit(EXIT_FAILURE);
	}

    return EXIT_SUCCESS;
}
