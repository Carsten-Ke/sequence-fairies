
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>

#include "../../../src/identifyName.hpp"

BOOST_AUTO_TEST_SUITE(identifyName_Test)


BOOST_AUTO_TEST_CASE( splitChar_Test )
{
	BioSeqDataLib::Sequence seq("seq1-1", "ACGTCT", "B7", "test sequence");
    auto result = splitCharIdentifier(seq, '-');
    BOOST_CHECK(result.second == ISOFORM_STATUS::MATCH);
    BOOST_CHECK_EQUAL(result.first, "seq1");
}

BOOST_AUTO_TEST_CASE( splitChar_doublechar_Test )
{
	BioSeqDataLib::Sequence seq("seq1-b-2", "ACGTCT", "B7", "test sequence");
    auto result = splitCharIdentifier(seq, '-');
    BOOST_CHECK(result.second == ISOFORM_STATUS::MATCH);
    BOOST_CHECK_EQUAL(result.first, "seq1-b");
}

BOOST_AUTO_TEST_CASE( splitChar_fail_Test )
{
	BioSeqDataLib::Sequence seq("seq1-1", "ACGTCT", "B7", "test sequence");
    auto result = splitCharIdentifier(seq, '.');
    BOOST_CHECK(result.second == ISOFORM_STATUS::NOMATCH);
    BOOST_CHECK_EQUAL(result.first, "seq1-1");
}

BOOST_AUTO_TEST_CASE( regex_Test)
{
    std::regex e(" gene[:=]\\s*([\\S]+)[\\s]*");
    BioSeqDataLib::Sequence seq("ENSSSCP00000002286.4", "ATCTG", "X", "pep primary_assembly:Sscrofa11.1:AEMK02000555.1:34878:35168:1 gene:ENSSSCG00000035087.2 tra");
    auto result = regexIdentifier(seq, e, false, false);
    BOOST_CHECK(result.second == ISOFORM_STATUS::MATCH);
    BOOST_CHECK_EQUAL(result.first, "ENSSSCG00000035087.2");
}

BOOST_AUTO_TEST_CASE( regex_Test_comment)
{
    std::regex e(" gene[:=]\\s*([\\S]+)[\\s]*");
    BioSeqDataLib::Sequence seq("ENSSSCP00000002286.4", "ATCTG", "X", "pep primary_assembly:Sscrofa11.1:AEMK02000555.1:34878:35168:1 gene:ENSSSCG00000035087.2 tra");
    auto result = regexIdentifier(seq, e, true, false);
    BOOST_CHECK(result.second == ISOFORM_STATUS::MATCH);
    BOOST_CHECK_EQUAL(result.first, "ENSSSCG00000035087.2");
}

BOOST_AUTO_TEST_CASE( regex_Test_name)
{
    std::regex e(" gene[:=]\\s*([\\S]+)[\\s]*");
    BioSeqDataLib::Sequence seq("ENSSSCP00000002286.4", "ATCTG", "X", "pep primary_assembly:Sscrofa11.1:AEMK02000555.1:34878:35168:1 gene:ENSSSCG00000035087.2 tra");
    auto result = regexIdentifier(seq, e, false, true);
    BOOST_CHECK(result.second == ISOFORM_STATUS::NOMATCH);
    BOOST_CHECK_EQUAL(result.first, "ENSSSCP00000002286.4");
}


BOOST_AUTO_TEST_SUITE_END()
