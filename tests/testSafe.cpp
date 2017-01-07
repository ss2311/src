#include <boost/test/unit_test.hpp>
#include <misc/safecontainer.h>
#include <vector>
#include <feeds/orderbook.h>

using namespace kalki;
using namespace std;


BOOST_AUTO_TEST_SUITE(ts_safe)

BOOST_AUTO_TEST_CASE(test_on_stack)
{
    Safe<vector<int>> svec;
    auto lk = svec.getLock();
    BOOST_CHECK_EQUAL(lk->size(), 0);
    lk->push_back(10);
    lk->push_back(20);
    lk->push_back(30);
    BOOST_CHECK_EQUAL(lk->size(), 3);

}

BOOST_AUTO_TEST_CASE(test_on_heap)
{
    auto svec = makeSafe<vector<int>>();
    auto lk = svec->getLock();
    BOOST_CHECK_EQUAL(lk->size(), 0);
    lk->push_back(10);
    lk->push_back(20);
    lk->push_back(30);
    BOOST_CHECK_EQUAL(lk->size(), 3);
}

BOOST_AUTO_TEST_CASE(test_orderbook)
{
    Safe<OrderBook> sbook(7);
    OrderBook * ob = nullptr;
    {
        auto lk = sbook.getLock();
        BOOST_CHECK_EQUAL(lk->m_numBids, 0);
        lk->m_numBids= 5;
        BOOST_CHECK_EQUAL(lk->m_numBids, 5);
        ob = lk.operator->(); // this is cheating :)
    }
    BOOST_CHECK_EQUAL(ob->m_numBids, 5);
}

BOOST_AUTO_TEST_SUITE_END()