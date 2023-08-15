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


#include "cmake_generated/project_version.h"
#include "concatenator.hpp"

namespace fs = std::filesystem;
namespace po = boost::program_options;


bool hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

int 
main (int argc, char *argv[])
{
	std::vector<fs::path> sequence_files;
	fs::path input_directory;
    fs::path outFile;
	std::string ending;
	po::options_description general("General options");
	general.add_options()
		("help,h", "Produces this help message")
		("in,i", po::value<std::vector<fs::path> >(&sequence_files)->multitoken()->value_name("FILE"), "The alignment files")
		("directory,d", po::value<fs::path>(&input_directory)->value_name("DIRECTORY"), "The directory with the alignment files")
		("ending,e", po::value<std::string>(&ending)->value_name("STRING")->default_value(".fa"), "The directory with the alignment files")
		("out,o", po::value<fs::path>(&outFile)->value_name("FILE")->default_value(""), "The output file")
	;

    std::string project_version(std::string(PROJECT_VERSION));
	po::options_description visibleOpts("concatenator " + project_version + " (C) 2023  Carsten Kemena\nThis program comes with ABSOLUTELY NO WARRANTY;\n\nAllowed options are displayed below");
	visibleOpts.add(general);

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
		std::cerr << "An error occurred parsing the command line: \n";
		std::cerr << e.what() << "\n";
		std::cerr << "Please use -h/--help for more information.\n";
		return EXIT_FAILURE;
	}

	BioSeqDataLib::SeqSetIOManager seqSetIO;
	seqSetIO.loadDefaultInputStrategies();
	seqSetIO.loadDefaultOutputStrategies();

	if (!input_directory.empty())
	{
		for (const auto & entry : fs::directory_iterator(input_directory))
		{
			if ((!fs::is_directory(entry)) && (hasEnding(entry.path().string(), ending)))
			{
				sequence_files.emplace_back(std::move(entry.path()));
			}
		}
	}

	auto nFiles = sequence_files.size();
	BioSeqDataLib::SequenceSet outSet;

    try
	{
		outSet = seqSetIO.read(sequence_files[0]);
		for (size_t i = 1; i < nFiles; ++i)
		{
        	BioSeqDataLib::SequenceSet seqSet = seqSetIO.read(sequence_files[i]);
			concatenate(outSet, seqSet);
		}
	}
	catch(std::ios_base::failure &exception)
	{
		std::cerr << exception.what() << std::endl;
		exit(EXIT_FAILURE);
	}


	try
	{
		if (outFile.empty())
			seqSetIO.write(outSet, "fasta", "");
		else
			seqSetIO.write(outSet, "fasta", outFile);
	}
	catch (std::ios_base::failure &exception)
	{
		std::cout << exception.what() << std::endl;
		exit(EXIT_FAILURE);
	}



    return EXIT_SUCCESS;
}
