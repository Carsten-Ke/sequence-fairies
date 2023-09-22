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
#include <filesystem>
#include <functional>
#include <iostream>
#include <map>
#include <regex>
#include <string>

// Boost
#include <boost/program_options.hpp>

// BSDL header
#include "../libs/BioSeqDataLib/src/sequence/SequenceSet.hpp"
#include "../libs/BioSeqDataLib/src/sequence/SeqSetIOManager.hpp"


#include "cmake_generated/project_version.h"
#include "isoformCleaner.hpp"
#include "identifyName.hpp"
#include "GFFParser.hpp"

namespace fs = std::filesystem;
namespace po = boost::program_options;


int main (int argc, char *argv[])
{
	fs::path sequenceFile;
    fs::path outFile;
    char splitChar;
	bool summary;
	po::options_description general("General options");
	general.add_options()
		("help,h", "Produces this help message")
		("in,i", po::value<fs::path>(&sequenceFile)->required()->multitoken()->value_name("FILE"), "The sequence file")
		("out,o", po::value<fs::path>(&outFile)->value_name("FILE"), "The output file")
		("split-char,s", po::value<char>(&splitChar)->value_name("CHAR")->default_value('-'), "The character used for splitting")
		("summary", po::value<bool>(&summary)->default_value(false)->zero_tokens(), "Displays a summary of the result")
	;

	fs::path gffFile;
	std::string level1,level2,level3;
	po::options_description gffOpts("GFF options");
	gffOpts.add_options()
		("gffFile", po::value<fs::path>(&gffFile)->default_value("")->value_name("FILE"), "A GFF file")
		("level1", po::value<std::string>(&level1)->default_value("gene"), "Level 1 term")
		("level2", po::value<std::string>(&level2)->default_value("mRNA"), "Level 2 term")	
		("level3", po::value<std::string>(&level3)->default_value("CDS"), "Level 3 term")
	;


	std::string regex, preset;
	bool searchComment, searchName;
	po::options_description regexOpts("Regex options");
	regexOpts.add_options()
		("regular,r", po::value<std::string>(&regex), "Regular expression")
		("name,n", po::value<bool>(&searchName)->default_value(false)->zero_tokens(), "Search name only")
		("comment,c", po::value<bool>(&searchComment)->default_value(false)->zero_tokens(), "Search comment only")
		("preset,p", po::value<std::string>(&preset), "Preset regex")
	;

    std::string project_version(std::string(PROJECT_VERSION));
	po::options_description visibleOpts("isoformCleaner " + project_version + " (C) 2019-2022  Carsten Kemena\nThis program comes with ABSOLUTELY NO WARRANTY;\n\nAllowed options are displayed below");
	visibleOpts.add(general).add(gffOpts).add(regexOpts);

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
		std::cerr << exception.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	IsoformCleaner isocleaner;
	std::set<std::string> names;
	if (!gffFile.empty())
	{
		auto genes = readGFF(gffFile, level1, level2, level3);
		names = longest(genes);
	}
	

	if (!preset.empty())
	{
		std::map<std::string, std::string> presetRegex = {{"flybase","parent=(FBgn[^ ,]+,)"}, {"gene"," gene[:=]\\s*([\\S]+)[\\s]*"}};
		regex = presetRegex[preset];
		if (regex.empty())
		{
			std::cerr << "Error: Preset '" << preset << "' is unknown!\n";
			exit(EXIT_FAILURE);
		}
	}

	if (!gffFile.empty())
	{
		std::function<std::pair<std::string, ISOFORM_STATUS>(BSDL::Sequence)> nameFunc =  std::bind(nameIdentifier, std::placeholders::_1, names);
		isocleaner.setGeneNameIdentifcator(nameFunc);
	} 
	else if (regex.empty())
	{
		std::function<std::pair<std::string, ISOFORM_STATUS>(BSDL::Sequence)> nameFunc =  std::bind(splitCharIdentifier, std::placeholders::_1, splitChar);
		isocleaner.setGeneNameIdentifcator(nameFunc);
	}
	else
	{
		const std::regex e(regex);
		std::function<std::pair<std::string, ISOFORM_STATUS>(BSDL::Sequence)> nameFunc =  std::bind(regexIdentifier, std::placeholders::_1, e, searchComment, searchName);
		isocleaner.setGeneNameIdentifcator(nameFunc);
	}

	auto outSet = isocleaner.clean(seqSet);
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

	if (summary)
	{
		std::cout << "# seqs in input: " << isocleaner.nSeqsBeforeCleaning() << "\n";
		std::cout << "# seqs in output: " << isocleaner.nSeqsAfterCleaning() << "\n";
		std::cout << "# seqs removed: " << isocleaner.nSeqsRemoved() << "\n";
		std::cout << "# warnings: " << isocleaner.warnings() << "\n";
	}

    return EXIT_SUCCESS;
}
