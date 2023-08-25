// C++
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <map>
#include <string>
#include <vector>



// Boost
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

// BSDL header
#include "../libs/BioSeqDataLib/src/domain/Domain.hpp"
#include "../libs/BioSeqDataLib/src/domain/DomainArrangementSet.hpp"
#include "../libs/BioSeqDataLib/src/sequence/SequenceSet.hpp"
#include "../libs/BioSeqDataLib/src/domain/DASetIOManager.hpp"
#include "../libs/BioSeqDataLib/src/sequence/SeqSetIOManager.hpp"


// other
#include "cmake_generated/project_version.h"

using namespace std;
namespace po = boost::program_options;
namespace fs = std::filesystem;
namespace BSDL = BioSeqDataLib;

int
main(int argc, char *argv[])
{
    std::string project_version(std::string(PROJECT_VERSION));
	vector<fs::path> domainFiles, sequenceFiles;
    fs::path outFile;
	size_t minLength;
	bool useEnvelope;
	std::string pattern, accession;
	po::options_description general("General options");
	general.add_options()
	("help,h", "Produces this help message")
	("domains,d", po::value<std::vector<fs::path> >(&domainFiles)->required()->multitoken()->value_name("FILE"), "The domain arrangement file")
	("in,i", po::value<std::vector<fs::path> >(&sequenceFiles)->required()->multitoken()->value_name("FILE"), "The sequence file")
    ("out,o", po::value<fs::path>(&outFile)->value_name("FILE"), "The output file")
	("name,n", po::value<std::string>(&pattern)->default_value("%s_%b-%e %d"), "Pattern to use for the sequence name")
	("id", po::value<std::string>(&accession)->value_name("ARG"), "The pfam-id to extract")
	("min-length", po::value<size_t>(&minLength)->default_value(0), "The minimum length to use")
	("use-envelope,e", po::value<bool>(&useEnvelope)->default_value(false)->zero_tokens(), "Use envelope coordinates if available")
	;

	po::options_description visibleOpts("extractDomains " + project_version + " (C) 2018-2023  Carsten Kemena\nThis program comes with ABSOLUTELY NO WARRANTY;\n\nAllowed options are displayed below");
	visibleOpts.add(general);

	try
	{
		po::variables_map vm;
		po::store(po::command_line_parser(argc, argv).options(visibleOpts).run(), vm);
		if (vm.count("help"))
		{
			cout << visibleOpts<< "\n";
			return EXIT_SUCCESS;
		}
		po::notify(vm);
	}
	catch (boost::program_options::error &e)
	{
		cerr << "An error occurred parsing the commandline: \n";
		cerr << e.what() << "\n";
		cerr << "Please use -h/--help for more information.\n";
		return EXIT_FAILURE;
	}


	try
	{
		BSDL::DASetIOManager<BSDL::DomainExt> daIO;
		daIO.loadDefaultInputStrategies();
		BSDL::SeqSetIOManager seqSetIO;
		seqSetIO.loadDefaultInputStrategies();
		seqSetIO.loadDefaultOutputStrategies();

		BSDL::SequenceSet domainSeqSet;
		for (size_t i=0; i<domainFiles.size(); ++i)
		{
			BSDL::DomainArrangementSet<BSDL::DomainExt> daSet;
			daSet = daIO.read(domainFiles[i]);
			BSDL::SequenceSet seqSet;
			seqSet = seqSetIO.read(sequenceFiles[i]);
			for (auto &seq : seqSet)
			{
				auto da = daSet[seq.name()];
				for (auto &domain : da)
				{
					unsigned long start = useEnvelope ? domain.envStart() : domain.start();
					unsigned long end = useEnvelope ? domain.envEnd() : domain.end();
					unsigned long length = useEnvelope ? domain.envLength() : domain.length();
					if ((!accession.empty()) && (domain.accession() != accession))
					{
						continue;
					}
					if ((minLength > 0) && (length < minLength))
					{
						continue;
					}
					
					BSDL::Sequence domSeq = subseq(seq, start, end);
					std::string name = pattern;
					boost::replace_all(name, "%s", seq.name());
					boost::replace_all(name, "%b", std::to_string(start+1));
					boost::replace_all(name, "%e", std::to_string(end+1));
					boost::replace_all(name, "%d", domain.accession());
					domSeq.name(std::move(name));
					domainSeqSet.push_back(std::move(domSeq));
				}
			}
		}
		seqSetIO.write(domainSeqSet, "fasta", outFile);
    }
	catch (std::ios_base::failure &exception)
	{
		cout << exception.what() << endl;
		exit(EXIT_FAILURE);
	}
	catch (...)
	{
		std::cerr << "Failed to extract domain." << endl;
		exit(2);
	}

}
