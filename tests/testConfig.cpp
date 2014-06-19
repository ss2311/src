#include <boost/test/unit_test.hpp>
#include <iostream>
#include <logger/logger.h>
#include <misc/config.h>

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

    BOOST_CHECK_EQUAL(cfg.getValue<string>("section1.field1",""), "value11");
    BOOST_CHECK_EQUAL(cfg.getValue<string>("section1.field2",""), "value12");
    BOOST_CHECK_EQUAL(cfg.getValue<string>("#section2.field1",""), "");
    BOOST_CHECK_EQUAL(cfg.getValue<int>("section2.field2",0), 459);
    BOOST_CHECK_EQUAL(cfg.getValue<double>("section2.field3",0), 123.456);
    BOOST_CHECK_EQUAL(cfg.size(),4);
}
