/**
 * @file gffStatisticsMain.cpp
 * @author Carsten Kemena (c.kemena@uni-muenster.de)
 * @brief 
 * @version 1.0
 * @date 2024-01-25
 * 
 * @copyright Copyright (c) 2024
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
#include <filesystem>
#include <functional>
#include <iostream>
#include <map>
#include <regex>
#include <string>

// Boost
#include <boost/program_options.hpp>

// BSDL header


#include "gffStatistics.hpp"

#include "cmake_generated/project_version.h"


namespace fs = std::filesystem;
namespace po = boost::program_options;


int main (int argc, char *argv[])
{
	std::vector<fs::path> gffFiles;
    fs::path outFile;
	po::options_description general("General options");
	general.add_options()
		("help,h", "Produces this help message")
		("in,i", po::value<std::vector<fs::path> >(&gffFiles)->required()->multitoken()->value_name("FILE(S)"), "The sequence file")
		("out,o", po::value<fs::path>(&outFile)->value_name("FILE"), "The output file")
	;

    std::string project_version(std::string(PROJECT_VERSION));
	po::options_description visibleOpts("gffStatistics " + project_version + " (C) 2024  Carsten Kemena\nThis program comes with ABSOLUTELY NO WARRANTY;\n\nAllowed options are displayed below");
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
		std::cerr << "An error occurred parsing the command line: " << e.what() << "\n";
		std::cerr << "Please use -h/--help for more information.\n";
		return EXIT_FAILURE;
	}

    try
	{
        std::cout << "file\tnGenes\tavgGene\tmedianGene\tnExons\tavgExon\tmedianExon\tnProteins" << "\n";
        for (auto file : gffFiles)
        {
            auto stats = createGFFStatistics(file);
            std::cout << file.string() << "\t" << stats.nGenes()  << "\t" << stats.averageGeneLength()  << "\t" <<
             stats.medianGeneLength() << "\t" << stats.nExons()  << "\t" << stats.averageExonLength()  << "\t" <<
             stats.medianExonLength() << "\t" << stats.nProteins() << "\n";
            
        }
	}
	catch(std::ios_base::failure &exception)
	{
		std::cerr << exception.what() << std::endl;
		exit(EXIT_FAILURE);
	}

    return EXIT_SUCCESS;
}
