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
#include <iomanip>
#include <iostream>
#include <map>
#include <regex>
#include <string>

// Boost
#include <boost/program_options.hpp>

// BSDL header
#include "../libs/BioSeqDataLib/src/utility/Output.hpp"

#include "gffStatistics.hpp"
#include "cmake_generated/project_version.h"


namespace fs = std::filesystem;
namespace po = boost::program_options;


int main (int argc, char *argv[])
{
	std::vector<fs::path> gffFiles;
    fs::path prefix, itolFile;
	po::options_description general("General options");
	general.add_options()
		("help,h", "Produces this help message")
		("in,i", po::value<std::vector<fs::path> >(&gffFiles)->required()->multitoken()->value_name("FILE(S)"), "The sequence file")
		("out,o", po::value<fs::path>(&prefix)->value_name("FILE")->required(), "The prefix for all output files")
		("itol", po::value<fs::path>(&itolFile)->value_name("FILE"), "An iTOL datasets file")
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
		std::map<std::string, std::vector<long> > proteinLengths;
        auto summaryName = std::filesystem::path(prefix.string() + "summary.txt");
        BioSeqDataLib::Output out(summaryName);
        out << "file\tnGenes\tavgGene\tmedianGene\tnExons\tavgExon\tmedianExon\tnCDS\tavgCDS\tmedianCDS" << "\n";
        out << std::fixed << std::setprecision(2);

		std::map<std::string, SummaryStatistics> allStats;
        for (auto file : gffFiles)
        {
            auto stats = createGFFStatistics(file);
            out << file.filename() << "\t" << 
            stats.nGenes() << "\t" << stats.averageGeneLength()  << "\t" << stats.medianGeneLength() << "\t" << 
            stats.nExons() << "\t" << stats.averageExonLength()  << "\t" << stats.medianExonLength() << "\t" << 
            stats.nProteins() << "\t" << stats.averageProteinLength()  << "\t" << stats.medianProteinLength() << "\n";
		    proteinLengths[file.filename()] = stats.getProteinLengths();
			allStats.emplace(file.stem(), stats);
        }
		auto protName = std::filesystem::path(prefix.string() + "proteinLengths.txt");
		BioSeqDataLib::Output outF(protName);
		for (auto elem : proteinLengths)
		{
			outF << elem.first; 
			for (auto value : elem.second)
			{
			outF << " " << value / 3;
			}
			outF << "\n";	
		}

		if (!itolFile.empty())
		{
			BioSeqDataLib::Output itolF(itolFile);
			itolF << "DATASET_SIMPLEBAR\n";
			itolF << "SEPARATOR COMMA\n";
			itolF << "DATASET_LABEL,GFFStatistics\n";
			itolF << "COLOR,#375e97\n";
			itolF << "FIELD_LABELS,Number of genes, number of proteins, avg. geneLength\n";
			itolF <<"DATA\n";
			for (auto elemPair : allStats)
			{
				auto &stat = elemPair.second;
				itolF << elemPair.first << "," << stat.nGenes() << "," << stat.nProteins() << "," << stat.averageProteinLength() << ";\n";
			}
		}
	}
	catch(std::ios_base::failure &exception)
	{
		std::cerr << exception.what() << std::endl;
		exit(EXIT_FAILURE);
	}

    return EXIT_SUCCESS;
}

