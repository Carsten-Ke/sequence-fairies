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
#include <iostream>
#include <string>

// Boost
#include <boost/program_options.hpp>

//BSDL header


#include "cmake_generated/project_version.h"
#include "isoformCleaner.hpp"

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
	visibleOpts.add(general).add(regexOpts);
	
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
		std::cerr << "An error occurred parsing the commandline: \n";
		std::cerr << e.what() << "\n";
		std::cerr << "Please use -h/--help for more information.\n";
		return EXIT_FAILURE;
	}


    return EXIT_SUCCESS;
}
