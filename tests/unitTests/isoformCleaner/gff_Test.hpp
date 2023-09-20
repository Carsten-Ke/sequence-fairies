
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>

#include "../../../src/GFFParser.hpp"

BOOST_AUTO_TEST_SUITE(gff_Test)


BOOST_AUTO_TEST_CASE( gffRead_Test )
{
	auto genes = readGFF("../tests/unitTests/isoformCleaner/test.gff3", "gEne", "mRNA", "CDS");
    BOOST_CHECK_EQUAL(genes.size(), 2);
    auto gene = genes.find("gene-1")->second;
    BOOST_CHECK_EQUAL(gene.longestTranscript(), "rna-2");

    gene = genes.find("gene-2")->second;
    BOOST_CHECK_EQUAL(gene.longestTranscript(), "rna-4");
}



BOOST_AUTO_TEST_SUITE_END()
