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
#include "ITOLWriter.hpp"
#include "cmake_generated/project_version.h"


namespace fs = std::filesystem;
namespace po = boost::program_options;


int main (int argc, char *argv[])
{
	std::vector<fs::path> gffFiles;
    fs::path outFolder;
    bool createITOLOutput;
	po::options_description general("General options");
	general.add_options()
		("help,h", "Produces this help message")
		("in,i", po::value<std::vector<fs::path> >(&gffFiles)->required()->multitoken()->value_name("FILE(S)"), "The sequence file")
		("out,o", po::value<fs::path>(&outFolder)->value_name("FOLDER")->required(), "A folder where all data will be saved in")
		("iTOL", po::bool_switch(&createITOLOutput), "Create output for iTOL")
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
        auto summaryName = outFolder / "summary.txt";
        BioSeqDataLib::Output out(summaryName);
        out << "file\tnGenes\tavgGene\tmedianGene\tnExons\tavgExon\tmedianExon\tnCDS\tavgCDS\tmedianCDS\tavgIntron\tmedianIntron" << "\n";
        out << std::fixed << std::setprecision(2);

		std::map<std::string, SummaryStatistics> allStats;
		std::vector<std::string> ids = {"gene", "cds", "exon", "intron"};
        for (auto file : gffFiles)
        {
			auto stats = createGFFStatistics(file);
            out << file.filename();
			for (auto id : ids)
			{
				out << "\t" << stats.nValues(id) << "\t" << stats.average(id) << "\t" << stats.median(id);
			}
			out << "\n";
			allStats.emplace(file.stem(), stats);
			proteinLengths[file.filename()] = stats.getValues("protein");
        }
		auto protName = std::filesystem::path(outFolder / "proteinLengths.txt");
		
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

		if (createITOLOutput)
		{
            auto itolDir = outFolder / "iTOL";
            std::filesystem::create_directories(itolDir);
            std::map<std::string, std::vector<ITOLValueSet> > itolValues;
            itolValues["nGenes"] = std::vector<ITOLValueSet>();
            itolValues["nProteins"] = std::vector<ITOLValueSet>();
            itolValues["avgProteinLength"] = std::vector<ITOLValueSet>();
            itolValues["medianProteinLength"] = std::vector<ITOLValueSet>();
			for (auto elemPair : allStats)
			{
				auto &stat = elemPair.second;
                itolValues["nGenes"].emplace_back(elemPair.first, stat.nValues("gene"), "");
                itolValues["nProteins"].emplace_back(elemPair.first, stat.nValues("protein"), "");
                itolValues["avgProteinLength"].emplace_back(elemPair.first, stat.average("protein"), "");
                itolValues["medianProteinLength"].emplace_back(elemPair.first, stat.median("protein"), "");
			}

            ITOLWriter itolW(itolDir / "geneNumber.txt", "# genes", "#2a4d69");
            itolW.writeValues(itolValues["nGenes"]);
            itolW.close();
            ITOLWriter itolW2(itolDir / "proteinNumber.txt", "# proteins", "#4b86b4");
            itolW2.writeValues(itolValues["nProteins"]);
            itolW2.close();
            ITOLWriter itolW3(itolDir / "avgProteinLength.txt", "avg protein length", "#abcbe3");
            itolW3.writeValues(itolValues["avgProteinLength"]);
            itolW3.close();
            ITOLWriter itolW4(itolDir / "medianProteinLength.txt", "median protein length", "#e7eff6");
            itolW4.writeValues(itolValues["medianProteinLength"]);
            itolW4.close();
		}
	}
	catch(std::ios_base::failure &exception)
	{
		std::cerr << exception.what() << std::endl;
		exit(EXIT_FAILURE);
	}

    return EXIT_SUCCESS;
}

