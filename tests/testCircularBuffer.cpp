#include <boost/test/unit_test.hpp>
#include <iostream>
#include <logger/logger.h>
#include <misc/cBuffer.h>

using namespace std;
using namespace logger;
using namespace tools;

///////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE(cbuf)

///////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE(testCB1)
{
	CircularQueue<int, 3> cbInt;

	// INITIAL CONDITIONS
	BOOST_CHECK_EQUAL(cbInt.size(), 4);
	BOOST_CHECK_EQUAL(cbInt.isEmpty(), true);
	BOOST_CHECK_EQUAL(cbInt.isFull(), false);
	BOOST_CHECK_EQUAL(cbInt.numElems(), 0);
}

///////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE(testCB2)
{
	CircularQueue<int, 3> cbInt;

	int tmp=0;

	// PRE-CONDITIONS
	BOOST_CHECK_EQUAL(cbInt.isEmpty(), true);
	BOOST_CHECK_EQUAL(cbInt.isFull(), false);
	BOOST_CHECK_EQUAL(cbInt.numElems(), 0);
	BOOST_CHECK_EQUAL(cbInt.read(tmp), false);

	cbInt.write(100);

	// POST-CONDITIONS
	BOOST_CHECK_EQUAL(cbInt.isEmpty(), false);
	BOOST_CHECK_EQUAL(cbInt.isFull(), false);
	BOOST_CHECK_EQUAL(cbInt.numElems(), 1);
	BOOST_CHECK_EQUAL(cbInt.read(tmp), true);
	BOOST_CHECK_EQUAL(tmp, 100);
}

///////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE(testCB3)
{
	CircularQueue<int, 3> cbInt;

	int tmp=0;

	// PRE-CONDITIONS
	BOOST_CHECK_EQUAL(cbInt.numElems(), 0);
	BOOST_CHECK_EQUAL(cbInt.read(tmp), false); // r = 0, w = 0

	cbInt.write(100); // r = 0, w = 1
	cbInt.write(200); // r = 0, w = 2
	cbInt.write(300); // r = 0, w = 3
	cbInt.write(400); // r = 1, w = 3
	cbInt.write(500); // r = 2, w = 3

	// POST-CONDITIONS
	BOOST_CHECK_EQUAL(cbInt.isEmpty(), false);
	BOOST_CHECK_EQUAL(cbInt.isFull(), true);
	BOOST_CHECK_EQUAL(cbInt.read(tmp), true);
	BOOST_CHECK_EQUAL(tmp, 100);

	BOOST_CHECK_EQUAL(cbInt.read(tmp), true);
	BOOST_CHECK_EQUAL(tmp, 200);

	BOOST_CHECK_EQUAL(cbInt.read(tmp), true);
	BOOST_CHECK_EQUAL(tmp, 300);

	BOOST_CHECK_EQUAL(cbInt.read(tmp), false);
}

///////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE(testCB4)
{
	CircularQueue<int, 3> cbInt;

	int tmp = 0;

	// PRE-CONDITIONS
	BOOST_CHECK_EQUAL(cbInt.numElems(), 0);
	BOOST_CHECK_EQUAL(cbInt.read(tmp), false); // r = 0, w = 0
	BOOST_CHECK_EQUAL(cbInt.front(tmp), false); 
	BOOST_CHECK_EQUAL(cbInt.back(tmp), false); 

	cbInt.write(100); // r = 0, w = 1
	BOOST_CHECK_EQUAL(cbInt.numElems(), 1);
	BOOST_CHECK_EQUAL(cbInt.front(tmp), true); 
	BOOST_CHECK_EQUAL(tmp, 100);
	BOOST_CHECK_EQUAL(cbInt.back(tmp), true); 
	BOOST_CHECK_EQUAL(tmp, 100);

	cbInt.write(200); // r = 0, w = 2
	BOOST_CHECK_EQUAL(cbInt.numElems(), 2);
	BOOST_CHECK_EQUAL(cbInt.front(tmp), true); 
	BOOST_CHECK_EQUAL(tmp, 100);
	BOOST_CHECK_EQUAL(cbInt.back(tmp), true); 
	BOOST_CHECK_EQUAL(tmp, 200);
	
	cbInt.write(300); // r = 0, w = 3
	BOOST_CHECK_EQUAL(cbInt.numElems(), 3);
	BOOST_CHECK_EQUAL(cbInt.isFull(), true);
	BOOST_CHECK_EQUAL(cbInt.front(tmp), true); 
	BOOST_CHECK_EQUAL(tmp, 100);
	BOOST_CHECK_EQUAL(cbInt.back(tmp), true); 
	BOOST_CHECK_EQUAL(tmp, 300);
	
	cbInt.write(400); // r = 0, w = 3
	BOOST_CHECK_EQUAL(cbInt.numElems(), 3);
	BOOST_CHECK_EQUAL(cbInt.front(tmp), true); 
	BOOST_CHECK_EQUAL(tmp, 100);
	BOOST_CHECK_EQUAL(cbInt.back(tmp), true); 
	BOOST_CHECK_EQUAL(tmp, 300);
	
	cbInt.write(500); // r = 0, w = 3
	BOOST_CHECK_EQUAL(cbInt.numElems(), 3);
	BOOST_CHECK_EQUAL(cbInt.front(tmp), true); 
	BOOST_CHECK_EQUAL(tmp, 100);
	BOOST_CHECK_EQUAL(cbInt.back(tmp), true); 
	BOOST_CHECK_EQUAL(tmp, 300);

	cbInt.write(600); // r = 0, w = 3
	BOOST_CHECK_EQUAL(cbInt.numElems(), 3);
	BOOST_CHECK_EQUAL(cbInt.front(tmp), true); 
	BOOST_CHECK_EQUAL(tmp, 100);
	BOOST_CHECK_EQUAL(cbInt.back(tmp), true); 
	BOOST_CHECK_EQUAL(tmp, 300);

	cbInt.write(700); // r = 0, w = 3
	BOOST_CHECK_EQUAL(cbInt.numElems(), 3);
	BOOST_CHECK_EQUAL(cbInt.front(tmp), true); 
	BOOST_CHECK_EQUAL(tmp, 100);
	BOOST_CHECK_EQUAL(cbInt.back(tmp), true); 
	BOOST_CHECK_EQUAL(tmp, 300);

	cbInt.read(tmp); // r = 1, w = 3
	BOOST_CHECK_EQUAL(tmp, 100);
	BOOST_CHECK_EQUAL(cbInt.numElems(), 2);
	BOOST_CHECK_EQUAL(cbInt.front(tmp), true); 
	BOOST_CHECK_EQUAL(tmp, 200);
	BOOST_CHECK_EQUAL(cbInt.back(tmp), true); 
	BOOST_CHECK_EQUAL(tmp, 300);

	cbInt.read(tmp); // r = 2, w = 3
	BOOST_CHECK_EQUAL(tmp, 200);
	BOOST_CHECK_EQUAL(cbInt.numElems(), 1);
	BOOST_CHECK_EQUAL(cbInt.front(tmp), true); 
	BOOST_CHECK_EQUAL(tmp, 300);
	BOOST_CHECK_EQUAL(cbInt.back(tmp), true); 
	BOOST_CHECK_EQUAL(tmp, 300);

	cbInt.read(tmp); // r = 3, w = 3
	BOOST_CHECK_EQUAL(tmp, 300);
	BOOST_CHECK_EQUAL(cbInt.numElems(), 0);
	BOOST_CHECK_EQUAL(cbInt.front(tmp), false); 
	BOOST_CHECK_EQUAL(cbInt.back(tmp), false); 
}

///////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE(testCB5)
{
	CircularQueue<int, 3> cbInt;

	int tmp = 0;

	// PRE-CONDITIONS
	BOOST_CHECK_EQUAL(cbInt.numElems(), 0);
	BOOST_CHECK_EQUAL(cbInt.read(tmp), false); // r = 0, w = 0

	cbInt.write(100); // r = 0, w = 1
	BOOST_CHECK_EQUAL(cbInt.numElems(), 1);
	cbInt.read(tmp); // r = 1, w = 1
	BOOST_CHECK_EQUAL(tmp, 100);
	BOOST_CHECK_EQUAL(cbInt.numElems(), 0);

	cbInt.write(200); // r = 1, w = 2
	BOOST_CHECK_EQUAL(cbInt.numElems(), 1);
	cbInt.read(tmp); // r= 2, w = 2
	BOOST_CHECK_EQUAL(tmp, 200);
	BOOST_CHECK_EQUAL(cbInt.numElems(), 0);
	
	cbInt.write(300); // r = 2, w = 3
	BOOST_CHECK_EQUAL(cbInt.numElems(), 1);
	cbInt.read(tmp); // r= 3, w = 3
	BOOST_CHECK_EQUAL(tmp, 300);
	BOOST_CHECK_EQUAL(cbInt.numElems(), 0);
	
	cbInt.write(400); // r = 3, w = 0
	BOOST_CHECK_EQUAL(cbInt.numElems(), 1);
	cbInt.read(tmp); // r= 0, w = 0
	BOOST_CHECK_EQUAL(tmp, 400);
	BOOST_CHECK_EQUAL(cbInt.numElems(), 0);

	cbInt.write(500); // r = 0, w = 1
	BOOST_CHECK_EQUAL(cbInt.numElems(), 1);
	cbInt.read(tmp); // r= 1, w = 1
	BOOST_CHECK_EQUAL(tmp, 500);
	BOOST_CHECK_EQUAL(cbInt.numElems(), 0);
}

///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE_END()

