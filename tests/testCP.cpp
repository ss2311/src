#include <boost/test/unit_test.hpp>
#include <iostream>
#include <logger/logger.h>
#include <misc/exception.h>
#include <fx/currencyPair.h>

using namespace std;
using namespace logger;

BOOST_AUTO_TEST_CASE(testcp1)
{
    
    Instrument::Config cfgEUR = { 2, "EUR" };
    Instrument::Config cfgUSD = { 1, "USD" };
    
}