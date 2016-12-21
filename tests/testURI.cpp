#include <boost/test/unit_test.hpp>
#include <iostream>
#include <misc/uri.h>


using namespace std;
using namespace kalki;

BOOST_AUTO_TEST_CASE(test_uri_file)
{
    Uri uri1("file:///Users/saurabh/Drawer/src/libs/feeds/orderbook.h"); 
    
    BOOST_CHECK_EQUAL(uri1.m_scheme, "file");
    BOOST_CHECK_EQUAL(uri1.m_path, "/Users/saurabh/Drawer/src/libs/feeds/orderbook.h");
    BOOST_CHECK_EQUAL(uri1.m_user, "");
    BOOST_CHECK_EQUAL(uri1.m_password, "");
    BOOST_CHECK_EQUAL(uri1.m_host, "");
    BOOST_CHECK_EQUAL(uri1.m_port, "");    
}

BOOST_AUTO_TEST_CASE(test_uri_tcp)
{
    Uri uri1("tcp://ssriva:cwx124@10.0.0.55:8080");
    Uri uri2("tcp://ssriva@10.0.0.55:8080");
    Uri uri3("tcp://10.0.0.55:8080");
    
    BOOST_CHECK_EQUAL(uri1.m_scheme, "tcp");
    BOOST_CHECK_EQUAL(uri1.m_user, "ssriva");
    BOOST_CHECK_EQUAL(uri1.m_password, "cwx124");
    BOOST_CHECK_EQUAL(uri1.m_host, "10.0.0.55");
    BOOST_CHECK_EQUAL(uri1.m_port, "8080");
    BOOST_CHECK_EQUAL(uri1.m_path, "");
    
    BOOST_CHECK_EQUAL(uri2.m_scheme, "tcp");
    BOOST_CHECK_EQUAL(uri2.m_user, "ssriva");
    BOOST_CHECK_EQUAL(uri2.m_password, "");
    BOOST_CHECK_EQUAL(uri2.m_host, "10.0.0.55");
    BOOST_CHECK_EQUAL(uri2.m_port, "8080");
    BOOST_CHECK_EQUAL(uri2.m_path, "");

    BOOST_CHECK_EQUAL(uri3.m_scheme, "tcp");
    BOOST_CHECK_EQUAL(uri3.m_user, "");
    BOOST_CHECK_EQUAL(uri3.m_password, "");
    BOOST_CHECK_EQUAL(uri3.m_host, "10.0.0.55");
    BOOST_CHECK_EQUAL(uri3.m_port, "8080");
    BOOST_CHECK_EQUAL(uri3.m_path, "");
}

BOOST_AUTO_TEST_CASE(test_uri_malformed)
{
    BOOST_CHECK_THROW(Uri uri1("file"), Exception);
    BOOST_CHECK_THROW(Uri uri2("file://"), Exception); 
    BOOST_CHECK_THROW(Uri uri3("tcp://@10.0.0.55:8080"), Exception); 
    BOOST_CHECK_THROW(Uri uri3("tcp://:cwx124@10.0.0.55:8080"), Exception);
    BOOST_CHECK_THROW(Uri uri4("tcp://10.0.0.55"), Exception);
    BOOST_CHECK_THROW(Uri uri5("tcp://"), Exception);
    BOOST_CHECK_THROW(Uri uri6("xyz://"), Exception);
}
