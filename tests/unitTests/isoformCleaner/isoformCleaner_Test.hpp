
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>

#include "../../../src/isoformCleaner.hpp"

BOOST_AUTO_TEST_SUITE(isoformCleaner_Test)


BOOST_AUTO_TEST_CASE( isoform_Test )
{
    BioSeqDataLib::SequenceSet seqSet;
    seqSet.emplace_back("seq1-1", "ACGTCTa", "B7", "test sequence");
    seqSet.emplace_back("seq1-2", "ACGTCTa", "B7", "test sequence");
    seqSet.emplace_back("seq1-3", "ACGTCTaa", "B7", "test sequence");
    seqSet.emplace_back("seq2-1", "ACGTCT", "B7", "test sequence");
    seqSet.emplace_back("seq2-2", "ACGTCTaa", "B7", "test sequence");
    seqSet.emplace_back("seq2-3", "ACGTCTa", "B7", "test sequence");
    seqSet.emplace_back("seq2-4", "ACGTCTa", "B7", "test sequence");
    seqSet.emplace_back("seq3", "ACGTCT", "B7", "test sequence");

    IsoformCleaner isocleaner;
    std::function<std::pair<std::string, ISOFORM_STATUS>(BSDL::Sequence)> nameFunc =  std::bind(splitCharIdentifier, std::placeholders::_1, '-');
	isocleaner.setGeneNameIdentifcator(nameFunc);
    auto cleanedSet = isocleaner.clean(seqSet);

    BOOST_CHECK_EQUAL(isocleaner.nSeqsBeforeCleaning(), 8);
    BOOST_CHECK_EQUAL(isocleaner.nSeqsAfterCleaning(), 3);
    BOOST_CHECK_EQUAL(isocleaner.nSeqsRemoved(), 5);
    BOOST_CHECK_EQUAL(isocleaner.warnings(), 1);
    BOOST_CHECK_EQUAL(cleanedSet[0].name(), "seq1-3");
    BOOST_CHECK_EQUAL(cleanedSet[1].name(), "seq2-2");
    BOOST_CHECK_EQUAL(cleanedSet[2].name(), "seq3");
}

BOOST_AUTO_TEST_SUITE_END()