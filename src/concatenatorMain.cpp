/**
 * @file isoformCleaner.cpp
 * @author Carsten kemena (c.kemena@wwu.de)
 * @brief File containing the isoformCleanerMain
 * @version 0.1
 * @date 2022-09-18
 * 
 * @copyright Copyright (c) 2022
 * 
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


int main (int argc, char *argv[])
{
	std::vector<fs::path> sequenceFiles;
    fs::path outFile;
	po::options_description general("General options");
	general.add_options()
		("help,h", "Produces this help message")
		("in,i", po::value<std::vector<fs::path> >(&sequenceFiles)->required()->multitoken()->value_name("FILE"), "The alignment files")
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

	auto nFiles = sequenceFiles.size();
	BioSeqDataLib::SequenceSet outSet;

    try
	{
		outSet = seqSetIO.read(sequenceFiles[0]);
		for (size_t i = 1; i < nFiles; ++i)
		{
        	BioSeqDataLib::SequenceSet seqSet = seqSetIO.read(sequenceFiles[i]);
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
