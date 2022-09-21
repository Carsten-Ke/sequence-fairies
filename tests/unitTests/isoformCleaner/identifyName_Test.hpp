
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>

#include "../../../src/identifyName.hpp"

BOOST_AUTO_TEST_SUITE(identifyName_Test)


BOOST_AUTO_TEST_CASE( splitChar_Test )
{
	BioSeqDataLib::Sequence<> seq("seq1-1", "ACGTCT", "B7", "test sequence");
    auto result = splitCharIdentifier(seq, '-');
    BOOST_CHECK_EQUAL(result.second, true);
    BOOST_CHECK_EQUAL(result.first, "seq1");
}

BOOST_AUTO_TEST_CASE( splitChar_doublechar_Test )
{
	BioSeqDataLib::Sequence<> seq("seq1-b-2", "ACGTCT", "B7", "test sequence");
    auto result = splitCharIdentifier(seq, '-');
    BOOST_CHECK_EQUAL(result.second, true);
    BOOST_CHECK_EQUAL(result.first, "seq1-b");
}

BOOST_AUTO_TEST_CASE( splitChar_fail_Test )
{
	BioSeqDataLib::Sequence<> seq("seq1-1", "ACGTCT", "B7", "test sequence");
    auto result = splitCharIdentifier(seq, '.');
    BOOST_CHECK_EQUAL(result.second, false);
    BOOST_CHECK_EQUAL(result.first, "seq1-1");
}

BOOST_AUTO_TEST_CASE( regex_Test )
{
    //regexIdentifier(const BSDL::Sequence<> &seq, const std::regex regEx, bool searchComment, bool searchName)
}

BOOST_AUTO_TEST_CASE( regex_Test_comment)
{
    //regexIdentifier(const BSDL::Sequence<> &seq, const std::regex regEx, bool searchComment, bool searchName)
}

BOOST_AUTO_TEST_CASE( regex_Test_name)
{
    //regexIdentifier(const BSDL::Sequence<> &seq, const std::regex regEx, bool searchComment, bool searchName)
}


BOOST_AUTO_TEST_SUITE_END()
