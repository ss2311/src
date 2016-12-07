#include <boost/test/unit_test.hpp>
#include <iostream>
#include <logger/logger.h>
#include <config/config.h>

using namespace std;
using namespace logger;
using namespace tools;

BOOST_AUTO_TEST_CASE(test_cfg_basics)
{
	stringstream ss;
	ss << "section1.field1=value11" << endl;
    ss << "section1.field2=value12" << endl;
    ss << "#section2.field1=value21" << endl;
    ss << " section2.field2 =   459 " << endl;
    ss << "                         " << endl;
    ss << " section2.field3 =   123.456 " << endl;

	Config cfg;
	cfg.readFile(ss);

    BOOST_CHECK_EQUAL(cfg.getValue("section1.field1",string()), "value11");
    BOOST_CHECK_EQUAL(cfg.getValue("section1.field2",string()), "value12");
    BOOST_CHECK_EQUAL(cfg.getValue("#section2.field1",string()), "");
    BOOST_CHECK_EQUAL(cfg.getValue("section2.field2",0), 459);
    BOOST_CHECK_EQUAL(cfg.getValue("section2.field3",0.0), 123.456);
    BOOST_CHECK_EQUAL(cfg.size(),4);
}

BOOST_AUTO_TEST_CASE(test_cfg_file)
{
	ofstream fs("currencypairs.cfg", ofstream::trunc);
    BOOST_REQUIRE_EQUAL(fs.good(), true);
    fs << "#id name   pips precision isdirect posLimit" << endl;
    fs << "100 EURUSD 0.0001 0.00001 true 10000000" << endl;
    fs << "101 AUDUSD 0.0001 0.00001 true 10000000" << endl;
    fs << "102 USDCHF 0.0001 0.00001 true 10000000" << endl;
    fs << "103 USDJPY 0.01 0.00001 true 10000000" << endl;

    fs.close();
}
