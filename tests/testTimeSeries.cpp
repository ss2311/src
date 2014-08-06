#include <boost/test/unit_test.hpp>
#include <iostream>
#include <logger/logger.h>
#include <fx/timeSeries.h>

using namespace std;
using namespace logger;
using namespace fx;

BOOST_AUTO_TEST_CASE(testTS1)
{
	TimeSeries<> ts1(4);
	ts1.insert(1.2559);
	ts1.insert(1.2560);
	ts1.insert(1.2561);
	ts1.insert(1.2562);
	ts1.insert(1.2563);

	BOOST_CHECK_EQUAL(ts1.getOldest().m_price, 1.2560);
	BOOST_CHECK_EQUAL(ts1.getRecent().m_price, 1.2563);
}
