
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>

#include "../../../src/concatenator.hpp"

BOOST_AUTO_TEST_SUITE(concatenator_Test)


BOOST_AUTO_TEST_CASE(createNamesIndex_simple_Test)
{
    BioSeqDataLib::SequenceSet set;
    set.emplace_back("seq1", "aaaa", "", "");
    set.emplace_back("seq2", "bbbb", "", "");
    auto names = createNamesIndex(set, "\r");

    BOOST_CHECK_EQUAL(names.size(), 2);
    BOOST_CHECK_EQUAL(names.at("seq1"), 0);
    BOOST_CHECK_EQUAL(names.at("seq2"), 1);
}


BOOST_AUTO_TEST_CASE(createNamesIndex_delimiter_Test)
{
    BioSeqDataLib::SequenceSet set1;
    set1.emplace_back("seq1", "aaaa", "", "");
    set1.emplace_back("seq2", "bbbb", "", "");

    BioSeqDataLib::SequenceSet set2;
    set2.emplace_back("seq2", "AAAA", "", "");
    set2.emplace_back("seq1", "BBBB", "", "");
    
    auto mapping = createMatches(set1, set2, false, "\r");
    auto it = mapping.begin();
    BOOST_CHECK_EQUAL(it->first, 0);
    BOOST_CHECK_EQUAL(it->second, 1);
    ++it;
    BOOST_CHECK_EQUAL(it->first, 1);
    BOOST_CHECK_EQUAL(it->second, 0);

}


BOOST_AUTO_TEST_SUITE_END()