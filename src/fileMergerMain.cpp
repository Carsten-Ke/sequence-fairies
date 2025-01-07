/**
 * @file fileMergerMain.cpp
 * @author Carsten Kemena (c.kemena@uni-muenster.de)
 * @brief 
 * @version 0.1
 * @date 2025-01-07
 * 
 * @copyright Copyright (c) 2025
 * 
 */

// C++
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <map>
#include <string>
#include <vector>

// Boost
#include <boost/program_options.hpp>

// BSDL header
#include "../libs/BioSeqDataLib/src/sequence/SequenceSet.hpp"
#include "../libs/BioSeqDataLib/src/sequence/SeqSetIOManager.hpp"


#include "cmake_generated/project_version.h"
#include "fileMerger.hpp"

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
	bool rename;
	std::string delimiter;
	std::vector<std::string> patterns;
	po::options_description general("General options");
	general.add_options()
		("help,h", "Produces this help message")
		("in,i", po::value<std::vector<fs::path> >(&sequence_files)->multitoken()->value_name("FILE"), "The alignment files")
		("directory,d", po::value<fs::path>(&input_directory)->value_name("DIRECTORY"), "The directory with the alignment files")
		("ending,e", po::value<std::string>(&ending)->value_name("STRING")->default_value(".fa"), "File ending to be used")
		("rename,r", po::value<bool>(&rename)->default_value(false)->zero_tokens(), "Renames sequences in case of conflict")
		("out,o", po::value<fs::path>(&outFile)->value_name("FILE")->default_value(""), "The output file")
	;
	
    std::string project_version(std::string(PROJECT_VERSION));
	po::options_description visibleOpts("fileMerger " + project_version + " (C) 2025  Carsten Kemena\nThis program comes with ABSOLUTELY NO WARRANTY;\n\nAllowed options are displayed below");
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
		for (const auto & entry : std::filesystem::directory_iterator(input_directory))
		{
			if ((!std::filesystem::is_directory(entry)) && (hasEnding(entry.path().string(), ending)))
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

	BioSeqDataLib::SequenceSet outSet;
    try
	{
		auto strategy = rename ? BioSeqDataLib::SequenceSet::ConflictResolutionStrategy::Rename : BioSeqDataLib::SequenceSet::ConflictResolutionStrategy::ThrowException;
		for (const auto &fileName : sequence_files)
		{
			std::map<std::string, int> names1, names2;
        	BioSeqDataLib::SequenceSet seqSet = seqSetIO.read(fileName);
			outSet.merge(seqSet, strategy);
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
