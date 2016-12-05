#include <boost/test/unit_test.hpp>
#include <misc/orderbook.h>
#include <iostream>

using namespace std;

BOOST_AUTO_TEST_CASE(test_price)
{
    Price px1(1.1234564);
    Price px2(2.123456789);
    Price px3(2.123456788);
    Price px4(2.123455789);
    Price px5(2.123456789);
    
    BOOST_CHECK_EQUAL(static_cast<int64_t>(px1), 1123456);
    BOOST_CHECK_EQUAL(static_cast<int64_t>(px2), 2123457);
    BOOST_CHECK_EQUAL(px2, px5);
    BOOST_CHECK_EQUAL(px2, px3); // 6 dp precision
    BOOST_CHECK_NE(px2, px4);
    BOOST_CHECK_GT(px2, px4);
    BOOST_CHECK_LE(px4, px2);
}

BOOST_AUTO_TEST_CASE(test_price_neg)
{
    Price px1(-1.1234564);
    Price px2(-2.123456789);
    Price px3(-2.123456788);
    Price px4(-2.123455789);
    Price px5(-2.123456789);
    
    BOOST_CHECK_EQUAL(static_cast<int64_t>(px1), -1123456);
    BOOST_CHECK_EQUAL(static_cast<int64_t>(px2), -2123457);
    BOOST_CHECK_EQUAL(px2, px5);
    BOOST_CHECK_EQUAL(px2, px3); // 6 dp precision
    BOOST_CHECK_NE(px2, px4);
    BOOST_CHECK_LE(px2, px4);
    BOOST_CHECK_GT(px4, px2);
}

BOOST_AUTO_TEST_CASE(test_ob_basic)
{
    OrderBookBuilder obb;
    obb.addLevel(Price(45.98), Size(10), OrderBook::Side::BID);
    obb.addLevel(45.68, Size(10), OrderBook::Side::BID);
    obb.addLevel(45.88, Size(10), OrderBook::Side::BID);
    obb.addLevel(45.96, Size(10), OrderBook::Side::BID);
    obb.addLevel(45.90, Size(10), OrderBook::Side::BID);
    obb.addLevel(45.58, Size(10), OrderBook::Side::BID);
    obb.addLevel(45.88, Size(10), OrderBook::Side::BID);
    obb.addLevel(45.98, Size(10), OrderBook::Side::BID);
    obb.addLevel(45.88, Size(10), OrderBook::Side::BID);
    obb.addLevel(45.78, Size(10), OrderBook::Side::BID);
    obb.addLevel(45.94, Size(10), OrderBook::Side::BID);
    obb.addLevel(45.92, Size(10), OrderBook::Side::BID);
    obb.addLevel(45.95, Size(1000), OrderBook::Side::BID);
    obb.addLevel(45.48, Size(1000), OrderBook::Side::BID);
    
    obb.addLevel(46.01, Size(10), OrderBook::Side::ASK);
    obb.addLevel(46.11, Size(10), OrderBook::Side::ASK);
    obb.addLevel(46.21, Size(10), OrderBook::Side::ASK);
    obb.addLevel(46.81, Size(10), OrderBook::Side::ASK);
    obb.addLevel(46.41, Size(1000), OrderBook::Side::ASK);
    obb.addLevel(46.01, Size(30), OrderBook::Side::ASK);
 
    OrderBook ob = obb.getOrderBook();
    cout << ob << endl; 
    BOOST_CHECK_EQUAL(ob.m_numBids, 10);
    BOOST_CHECK_EQUAL(ob.m_numAsks, 5); 
    BOOST_CHECK_EQUAL(ob.m_bidPx[0], Price(45.98));
    BOOST_CHECK_EQUAL(ob.m_bidSz[0], 20);
    BOOST_CHECK_EQUAL(ob.m_bidPx[9], Price(45.58));
    BOOST_CHECK_EQUAL(ob.m_bidSz[9], 10);
  
    BOOST_CHECK_EQUAL(ob.m_askPx[0], Price(46.01));
    BOOST_CHECK_EQUAL(ob.m_askSz[0], 40);
    BOOST_CHECK_EQUAL(ob.m_askPx[4], Price(46.81));
    BOOST_CHECK_EQUAL(ob.m_askSz[4], 10);
}

BOOST_AUTO_TEST_CASE(test_ob_add_del)
{
    OrderBookBuilder obb;
    {
    obb.addLevel(45.68, 10, OrderBook::Side::BID);
    OrderBook ob = obb.getOrderBook();
    BOOST_CHECK_EQUAL(ob.m_numBids, 1);
    BOOST_CHECK_EQUAL(ob.m_numAsks, 0);    
    BOOST_CHECK_EQUAL(ob.m_bidPx[0], Price(45.68));
    BOOST_CHECK_EQUAL(ob.m_bidSz[0], 10);
    }
    {
    obb.addLevel(45.68, 20, OrderBook::Side::BID);
    OrderBook ob = obb.getOrderBook();
    BOOST_CHECK_EQUAL(ob.m_numBids, 1);
    BOOST_CHECK_EQUAL(ob.m_bidPx[0], Price(45.68));
    BOOST_CHECK_EQUAL(ob.m_bidSz[0], 30);
    }
    {
    obb.removeLevel(45.68, 15, OrderBook::Side::BID);
    OrderBook ob = obb.getOrderBook();
    BOOST_CHECK_EQUAL(ob.m_numBids, 1);
    BOOST_CHECK_EQUAL(ob.m_bidPx[0], Price(45.68));
    BOOST_CHECK_EQUAL(ob.m_bidSz[0], 15);
    }
    {
    obb.removeLevel(45.68, 5);
    OrderBook ob = obb.getOrderBook();
    BOOST_CHECK_EQUAL(ob.m_numBids, 1);
    BOOST_CHECK_EQUAL(ob.m_bidPx[0], Price(45.68));
    BOOST_CHECK_EQUAL(ob.m_bidSz[0], 10);
    }
    {
    obb.removeLevel(45.68);
    OrderBook ob = obb.getOrderBook();
    BOOST_CHECK_EQUAL(ob.m_numBids, 0);
    }    
}