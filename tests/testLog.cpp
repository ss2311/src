#include <boost/test/unit_test.hpp>
#include <iostream>
#include <logger/logger.h>
#include <misc/exception.h>

using namespace std;
using namespace logger;

BOOST_AUTO_TEST_CASE(test1)
{
    string str1 = "str1-data";
    const char* str3 = str1.c_str();
    char str2[64];
    strcpy(str2, str1.c_str());
    
    Logger<> log;
    Logger<> log2;
    log.getSink().setFile("/tmp/ss.log");
    log.logDBG(200 << " xyz");
    log.logWARN("str3=" << str3);
    log2.logINFO("first log from log2");
    log.logWARN("Testing in progress ...");
    log.logERR("Saurabh " << "Srivastava " << 6902 << " CT");
    log.logINFO("contents of str1=" << str1);
    log2.logINFO("last log from from log2");
    log.logINFO("contents of str2=" << str2);
	BOOST_CHECK_EQUAL(true, true);
}

BOOST_AUTO_TEST_CASE(test_exception)
{
    Logger<> log;
    string name = "xyz";
    unsigned id = 589;
    try {
//        throw Exception("xyz", __FILENAME__, __LINE__);
        throw EXCEPTION("name:" << name << ", id:" << id << " dbl:" << 123.456);
    } catch (const Exception& err_) {
        log.logERR(err_.what());
        log.logERR(err_);
    }
}
