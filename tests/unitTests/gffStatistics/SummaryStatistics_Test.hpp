
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>

#include "../../../src/SummaryStatistics.hpp"

BOOST_AUTO_TEST_SUITE(SummaryStatistics_Test)


BOOST_AUTO_TEST_CASE(GeneStats)
{
    SummaryStatistics stats;
    stats.addValue("gene", 1);
    stats.addValue("gene", 3);
    stats.addValue("gene", 2);
    BOOST_CHECK_EQUAL(stats.average("gene"), 2);
    BOOST_CHECK_EQUAL(stats.median("gene"), 2);

    stats.addValue("gene", 5);
    BOOST_CHECK_EQUAL(stats.average("gene"), 2.75);
    BOOST_CHECK_EQUAL(stats.median("gene"), 2.5);
    BOOST_CHECK_EQUAL(stats.nValues("gene"), 4);
}


BOOST_AUTO_TEST_SUITE_END()