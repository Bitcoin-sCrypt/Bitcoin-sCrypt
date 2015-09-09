#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include "util.h"

BOOST_AUTO_TEST_SUITE(getarg_tests)

static void
ResetArgs(const std::string& strArg)
{
    std::vector<std::string> vecArg;
    boost::split(vecArg, strArg, boost::is_space(), boost::token_compress_on);

    // Insert dummy executable name:
    vecArg.insert(vecArg.begin(), "testbitcoin");

    // Convert to char*:
    std::vector<const char*> vecChar;
    BOOST_FOREACH(std::string& s, vecArg)
        vecChar.push_back(s.c_str());

    ParseParameters(vecChar.size(), &vecChar[0]);
}

BOOST_AUTO_TEST_CASE(boolarg)
{
    ResetArgs("-btc");
    BOOST_CHECK(GetBoolArg("-btc"));
    BOOST_CHECK(GetBoolArg("-btc", false));
    BOOST_CHECK(GetBoolArg("-btc", true));

    BOOST_CHECK(!GetBoolArg("-fo"));
    BOOST_CHECK(!GetBoolArg("-fo", false));
    BOOST_CHECK(GetBoolArg("-fo", true));

    BOOST_CHECK(!GetBoolArg("-btco"));
    BOOST_CHECK(!GetBoolArg("-btco", false));
    BOOST_CHECK(GetBoolArg("-btco", true));

    ResetArgs("-btc=0");
    BOOST_CHECK(!GetBoolArg("-btc"));
    BOOST_CHECK(!GetBoolArg("-btc", false));
    BOOST_CHECK(!GetBoolArg("-btc", true));

    ResetArgs("-btc=1");
    BOOST_CHECK(GetBoolArg("-btc"));
    BOOST_CHECK(GetBoolArg("-btc", false));
    BOOST_CHECK(GetBoolArg("-btc", true));

    // New 0.6 feature: auto-map -nosomething to !-something:
    ResetArgs("-nobtc");
    BOOST_CHECK(!GetBoolArg("-btc"));
    BOOST_CHECK(!GetBoolArg("-btc", false));
    BOOST_CHECK(!GetBoolArg("-btc", true));

    ResetArgs("-nobtc=1");
    BOOST_CHECK(!GetBoolArg("-btc"));
    BOOST_CHECK(!GetBoolArg("-btc", false));
    BOOST_CHECK(!GetBoolArg("-btc", true));

    ResetArgs("-btc -nobtc");  // -btc should win
    BOOST_CHECK(GetBoolArg("-btc"));
    BOOST_CHECK(GetBoolArg("-btc", false));
    BOOST_CHECK(GetBoolArg("-btc", true));

    ResetArgs("-btc=1 -nobtc=1");  // -btc should win
    BOOST_CHECK(GetBoolArg("-btc"));
    BOOST_CHECK(GetBoolArg("-btc", false));
    BOOST_CHECK(GetBoolArg("-btc", true));

    ResetArgs("-btc=0 -nobtc=0");  // -btc should win
    BOOST_CHECK(!GetBoolArg("-btc"));
    BOOST_CHECK(!GetBoolArg("-btc", false));
    BOOST_CHECK(!GetBoolArg("-btc", true));

    // New 0.6 feature: treat -- same as -:
    ResetArgs("--btc=1");
    BOOST_CHECK(GetBoolArg("-btc"));
    BOOST_CHECK(GetBoolArg("-btc", false));
    BOOST_CHECK(GetBoolArg("-btc", true));

    ResetArgs("--nobtc=1");
    BOOST_CHECK(!GetBoolArg("-btc"));
    BOOST_CHECK(!GetBoolArg("-btc", false));
    BOOST_CHECK(!GetBoolArg("-btc", true));

}

BOOST_AUTO_TEST_CASE(stringarg)
{
    ResetArgs("");
    BOOST_CHECK_EQUAL(GetArg("-btc", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-btc", "eleven"), "eleven");

    ResetArgs("-btc -bar");
    BOOST_CHECK_EQUAL(GetArg("-btc", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-btc", "eleven"), "");

    ResetArgs("-btc=");
    BOOST_CHECK_EQUAL(GetArg("-btc", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-btc", "eleven"), "");

    ResetArgs("-btc=11");
    BOOST_CHECK_EQUAL(GetArg("-btc", ""), "11");
    BOOST_CHECK_EQUAL(GetArg("-btc", "eleven"), "11");

    ResetArgs("-btc=eleven");
    BOOST_CHECK_EQUAL(GetArg("-btc", ""), "eleven");
    BOOST_CHECK_EQUAL(GetArg("-btc", "eleven"), "eleven");

}

BOOST_AUTO_TEST_CASE(intarg)
{
    ResetArgs("");
    BOOST_CHECK_EQUAL(GetArg("-btc", 11), 11);
    BOOST_CHECK_EQUAL(GetArg("-btc", 0), 0);

    ResetArgs("-btc -bar");
    BOOST_CHECK_EQUAL(GetArg("-btc", 11), 0);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 0);

    ResetArgs("-btc=11 -bar=12");
    BOOST_CHECK_EQUAL(GetArg("-btc", 0), 11);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 12);

    ResetArgs("-btc=NaN -bar=NotANumber");
    BOOST_CHECK_EQUAL(GetArg("-btc", 1), 0);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 0);
}

BOOST_AUTO_TEST_CASE(doubledash)
{
    ResetArgs("--btc");
    BOOST_CHECK_EQUAL(GetBoolArg("-btc"), true);

    ResetArgs("--btc=verbose --bar=1");
    BOOST_CHECK_EQUAL(GetArg("-btc", ""), "verbose");
    BOOST_CHECK_EQUAL(GetArg("-bar", 0), 1);
}

BOOST_AUTO_TEST_CASE(boolargno)
{
    ResetArgs("-nobtc");
    BOOST_CHECK(!GetBoolArg("-btc"));
    BOOST_CHECK(!GetBoolArg("-btc", true));
    BOOST_CHECK(!GetBoolArg("-btc", false));

    ResetArgs("-nobtc=1");
    BOOST_CHECK(!GetBoolArg("-btc"));
    BOOST_CHECK(!GetBoolArg("-btc", true));
    BOOST_CHECK(!GetBoolArg("-btc", false));

    ResetArgs("-nobtc=0");
    BOOST_CHECK(GetBoolArg("-btc"));
    BOOST_CHECK(GetBoolArg("-btc", true));
    BOOST_CHECK(GetBoolArg("-btc", false));

    ResetArgs("-btc --nobtc");
    BOOST_CHECK(GetBoolArg("-btc"));

    ResetArgs("-nobtc -btc"); // btc always wins:
    BOOST_CHECK(GetBoolArg("-btc"));
}

BOOST_AUTO_TEST_SUITE_END()
