#include <boost/test/unit_test.hpp>
#include <iostream>
#include <string>
#include <logger/logger.h>
#include <misc/mempool.h>

using namespace std;
using namespace logger;
using namespace tools;

///////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE(mempool)

struct TestData
{
	string name;
	int id;
	double price;
};
	

///////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE(testMP1)
{
	MemPool<TestData, 5> memPool;

	// INITIAL CONDITIONS
	BOOST_CHECK_EQUAL(memPool.size(), 5);
	BOOST_CHECK_EQUAL(memPool.numElems(), 0);
}

///////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE(testMP2)
{
	MemPool<TestData, 5> memPool;

	// PRE-CONDITIONS
	BOOST_CHECK_EQUAL(memPool.size(), 5);
	BOOST_CHECK_EQUAL(memPool.numElems(), 0);

	auto& mem = memPool.alloc();

	// POST-CONDITIONS
	BOOST_CHECK_EQUAL(memPool.numElems(), 1);
}

///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE_END()

