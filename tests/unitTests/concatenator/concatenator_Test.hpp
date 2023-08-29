
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>

#include "../../../src/concatenator.hpp"

BOOST_AUTO_TEST_SUITE(concatenator_Test)


BOOST_AUTO_TEST_CASE(createNamesIndex_wholeName_Test)
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
    
    auto names1 = createNamesIndex(set1, "\r");
    auto names2 = createNamesIndex(set2, "\r");
    auto mapping = createMatches(names1, "", names2, "", false);
    auto it = mapping.begin();
    BOOST_CHECK_EQUAL(it->first, 0);
    BOOST_CHECK_EQUAL(it->second, 1);
    ++it;
    BOOST_CHECK_EQUAL(it->first, 1);
    BOOST_CHECK_EQUAL(it->second, 0);
}


BOOST_AUTO_TEST_CASE(createNamesIndex_pattern_Test)
{
    BioSeqDataLib::SequenceSet set;
    set.emplace_back("seq1_abc", "aaaa", "", "");
    set.emplace_back("abc_seq2", "bbbb", "", "");
    set.emplace_back("abc_seq3_abc", "bbbb", "", "");
    auto names = createNamesIndex(set, {"seq2", "seq3", "seq1"});

    BOOST_CHECK_EQUAL(names.size(), 3);
    BOOST_CHECK_EQUAL(names.at("seq1"), 0);
    BOOST_CHECK_EQUAL(names.at("seq2"), 1);
    BOOST_CHECK_EQUAL(names.at("seq3"), 2);
}

bool correctMessage(const std::runtime_error& ex)
{
    BOOST_CHECK_EQUAL(ex.what(), std::string("Error! Duplicated sequence name 'seq1' in file 'Test File' found!"));
    return true;
}

BOOST_AUTO_TEST_CASE(createNamesIndex_pattern_duplicates_Test)
{
    BioSeqDataLib::SequenceSet set;
    set.emplace_back("seq1_abc", "aaaa", "", "");
    set.emplace_back("abc_seq1", "bbbb", "", "");
    set.emplace_back("abc_seq2_abc", "bbbb", "", "");
    set.file("Test File");

    std::vector<std::string> patterns = {"seq2", "seq1"};
    BOOST_CHECK_EXCEPTION(createNamesIndex(set, patterns), std::runtime_error, correctMessage);


}


BOOST_AUTO_TEST_SUITE_END()