#include <boost/test/unit_test.hpp>
#include <iostream>
#include <logger/logger.h>
#include <misc/exception.h>
#include <misc/double.h>

using namespace std;
using namespace logger;
using namespace tools;

BOOST_AUTO_TEST_CASE(test_pow)
{
    BOOST_CHECK_EQUAL(power(10,0), 1);
    BOOST_CHECK_EQUAL(power(10,1), 10);
    BOOST_CHECK_EQUAL(power(10,2), 100);
    BOOST_CHECK_EQUAL(power(10,3), 1000);
    BOOST_CHECK_EQUAL(power(10,4), 10000);
}

BOOST_AUTO_TEST_CASE(test_dbl)
{
    {
        Double<6> dbl(1.123);
        BOOST_CHECK_CLOSE(dbl.asDouble(), 1.123000, 0.0001);
    }
    {
        Double<6> dbl(10.0/3.0);
        BOOST_CHECK_CLOSE(dbl.asDouble(), 3.333333, 0.0001);
    }
    {
        Double<6> dbl(4.5/1.5);
        BOOST_CHECK_CLOSE(dbl.asDouble(), 3.000000, 0.0001);
    }
}

BOOST_AUTO_TEST_CASE(test_rounding)
{
    {
        Double<3> dbl = 1.1231;
        BOOST_CHECK_CLOSE(dbl.asDouble(), 1.1230, 0.0001);
    }
    {
        Double<3> dbl = 1.1239;
        BOOST_CHECK_CLOSE(dbl.asDouble(), 1.1240, 0.0001);
    }
    {
        Double<3> dbl = -1.1231;
        BOOST_CHECK_CLOSE(dbl.asDouble(), -1.1230, 0.0001);
    }
    {
        Double<3> dbl = -1.1239;
        BOOST_CHECK_CLOSE(dbl.asDouble(), -1.1240, 0.0001);
    }
    {
        Double<3> dbl = 1.0049999;
        BOOST_CHECK_CLOSE(dbl.asDouble(), 1.005, 0.0001);
    }
    {
        Double<3> dbl = -1.0049999;
        BOOST_CHECK_CLOSE(dbl.asDouble(), -1.005, 0.0001);
    }
    {
        Double<3> dbl = 1.0045;
        BOOST_CHECK_CLOSE(dbl.asDouble(), 1.005, 0.0001);
    }
    {
        Double<3> dbl = -1.0045;
        BOOST_CHECK_CLOSE(dbl.asDouble(), -1.005, 0.0001);
    }
}

BOOST_AUTO_TEST_CASE(test_truncating)
{
    typedef Double<3> Dbl;
    {
        Dbl dbl(1.1231, RoundType::Trunc);
        BOOST_CHECK_CLOSE(dbl.asDouble(), 1.1230, 0.0001);
    }
    {
        Dbl dbl(1.1239, RoundType::Trunc);
        BOOST_CHECK_CLOSE(dbl.asDouble(), 1.1230, 0.0001);
    }
    {
        Dbl dbl(-1.1231, RoundType::Trunc);
        BOOST_CHECK_CLOSE(dbl.asDouble(), -1.1230, 0.0001);
    }
    {
        Dbl dbl(-1.1239, RoundType::Trunc);
        BOOST_CHECK_CLOSE(dbl.asDouble(), -1.1230, 0.0001);
    }
    {
        Dbl dbl(1.0049999, RoundType::Trunc);
        BOOST_CHECK_CLOSE(dbl.asDouble(), 1.004, 0.0001);
    }
    {
        Dbl dbl(-1.0049999, RoundType::Trunc);
        BOOST_CHECK_CLOSE(dbl.asDouble(), -1.004, 0.0001);
    }
    {
        Dbl dbl(1.0045, RoundType::Trunc);
        BOOST_CHECK_CLOSE(dbl.asDouble(), 1.004, 0.0001);
    }
    {
        Dbl dbl(-1.0045, RoundType::Trunc);
        BOOST_CHECK_CLOSE(dbl.asDouble(), -1.004, 0.0001);
    }
}

BOOST_AUTO_TEST_CASE(test_RoundUp)
{
    typedef Double<3> Dbl;
    {
        Dbl dbl(1.1231, RoundType::Up);
        BOOST_CHECK_CLOSE(dbl.asDouble(), 1.1240, 0.0001);
    }
    {
        Dbl dbl(1.1239, RoundType::Up);
        BOOST_CHECK_CLOSE(dbl.asDouble(), 1.1240, 0.0001);
    }
    {
        Dbl dbl(-1.1231, RoundType::Up);
        BOOST_CHECK_CLOSE(dbl.asDouble(), -1.1230, 0.0001);
    }
    {
        Dbl dbl(-1.1239, RoundType::Up);
        BOOST_CHECK_CLOSE(dbl.asDouble(), -1.1230, 0.0001);
    }
    {
        Dbl dbl(1.0049999, RoundType::Up);
        BOOST_CHECK_CLOSE(dbl.asDouble(), 1.005, 0.0001);
    }
    {
        Dbl dbl(-1.0049999, RoundType::Up);
        BOOST_CHECK_CLOSE(dbl.asDouble(), -1.004, 0.0001);
    }
    {
        Dbl dbl(1.0045, RoundType::Up);
        BOOST_CHECK_CLOSE(dbl.asDouble(), 1.005, 0.0001);
    }
    {
        Dbl dbl(-1.0045, RoundType::Up);
        BOOST_CHECK_CLOSE(dbl.asDouble(), -1.004, 0.0001);
    }
}

BOOST_AUTO_TEST_CASE(test_RoundDn)
{
    typedef Double<3> Dbl;
    {
        Dbl dbl(1.1231, RoundType::Down);
        BOOST_CHECK_CLOSE(dbl.asDouble(), 1.1230, 0.0001);
    }
    {
        Dbl dbl(1.1239, RoundType::Down);
        BOOST_CHECK_CLOSE(dbl.asDouble(), 1.1230, 0.0001);
    }
    {
        Dbl dbl(-1.1231, RoundType::Down);
        BOOST_CHECK_CLOSE(dbl.asDouble(), -1.1240, 0.0001);
    }
    {
        Dbl dbl(-1.1239, RoundType::Down);
        BOOST_CHECK_CLOSE(dbl.asDouble(), -1.1240, 0.0001);
    }
    {
        Dbl dbl(1.0049999, RoundType::Down);
        BOOST_CHECK_CLOSE(dbl.asDouble(), 1.004, 0.0001);
    }
    {
        Dbl dbl(-1.0049999, RoundType::Down);
        BOOST_CHECK_CLOSE(dbl.asDouble(), -1.005, 0.0001);
    }
    {
        Dbl dbl(1.0045, RoundType::Down);
        BOOST_CHECK_CLOSE(dbl.asDouble(), 1.004, 0.0001);
    }
    {
        Dbl dbl(-1.0045, RoundType::Down);
        BOOST_CHECK_CLOSE(dbl.asDouble(), -1.005, 0.0001);
    }
}

BOOST_AUTO_TEST_CASE(test_RoundRecursive)
{
    Double<6> dbl6(1.123446);
    BOOST_CHECK_CLOSE(dbl6.asDouble(), 1.123446, 0.0001);

    Double<5> dbl5(dbl6.asDouble());
    BOOST_CHECK_CLOSE(dbl5.asDouble(), 1.12345, 0.0001);

    Double<4> dbl4(dbl5.asDouble());
    BOOST_CHECK_CLOSE(dbl4.asDouble(), 1.1235, 0.0001);

    Double<3> dbl3(dbl4.asDouble());
    BOOST_CHECK_CLOSE(dbl3.asDouble(), 1.124, 0.0001);

    Double<2> dbl2(dbl3.asDouble());
    BOOST_CHECK_CLOSE(dbl2.asDouble(), 1.12, 0.0001);

    Double<1> dbl1(dbl2.asDouble());
    BOOST_CHECK_CLOSE(dbl1.asDouble(), 1.1, 0.0001);

    Double<0> dbl0(dbl1.asDouble());
    BOOST_CHECK_CLOSE(dbl0.asDouble(), 1.0, 0.0001);

}