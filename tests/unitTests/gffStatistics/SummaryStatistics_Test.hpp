
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>

#include "../../../src/SummaryStatistics.hpp"

BOOST_AUTO_TEST_SUITE(SummaryStatistics_Test)


BOOST_AUTO_TEST_CASE(GeneStats)
{
    SummaryStatistics stats;
    stats.addGeneLength(1);
    stats.addGeneLength(3);
    stats.addGeneLength(2);
    BOOST_CHECK_EQUAL(stats.averageGeneLength(), 2);
    BOOST_CHECK_EQUAL(stats.medianGeneLength(), 2);

    stats.addGeneLength(5);
    BOOST_CHECK_EQUAL(stats.averageGeneLength(), 2.75);
    BOOST_CHECK_EQUAL(stats.medianGeneLength(), 2.5);
    BOOST_CHECK_EQUAL(stats.nGenes(), 4);
}

BOOST_AUTO_TEST_CASE(ProteinStats)
{
    SummaryStatistics stats;
    stats.addProteinLength(1);
    stats.addProteinLength(3);
    stats.addProteinLength(2);
    BOOST_CHECK_EQUAL(stats.averageProteinLength(), 2);
    BOOST_CHECK_EQUAL(stats.medianProteinLength(), 2);

    stats.addProteinLength(5);
    BOOST_CHECK_EQUAL(stats.averageProteinLength(), 2.75);
    BOOST_CHECK_EQUAL(stats.medianProteinLength(), 2.5);
    BOOST_CHECK_EQUAL(stats.nProteins(), 4);
}

BOOST_AUTO_TEST_CASE(ExonStats)
{
    SummaryStatistics stats;
    stats.addExonLength(1);
    stats.addExonLength(3);
    stats.addExonLength(2);
    BOOST_CHECK_EQUAL(stats.averageExonLength(), 2);
    BOOST_CHECK_EQUAL(stats.medianExonLength(), 2);

    stats.addExonLength(5);
    BOOST_CHECK_EQUAL(stats.averageExonLength(), 2.75);
    BOOST_CHECK_EQUAL(stats.medianExonLength(), 2.5);
    BOOST_CHECK_EQUAL(stats.nExons(), 4);

}

BOOST_AUTO_TEST_SUITE_END()