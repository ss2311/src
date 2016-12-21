#include <boost/test/unit_test.hpp>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <utility>  // pair
#include <tuple>    // tie
#include <misc/exception.h>
#include <misc/utils.h>


namespace kalki {
struct Uri {
    // Refer https://en.wikipedia.org/wiki/Uniform_Resource_Identifier#Syntax
    // scheme:[//[user:password@]host[:port]][/]path[?query][#fragment]
    // Examples:
    //  file:///Users/saurabh/Drawer/src/libs/feeds/orderbook.h
    //  tcp://ssriva:cwx124@10.0.0.55:8080
    std::string m_scheme, m_user, m_password, m_host, m_port, m_path; 
    friend std::ostream& operator << (std::ostream& os_, const Uri& uri_) {
        return os_  
            << " s:'" << uri_.m_scheme  
            << "' u:'" << uri_.m_user
            << "' p:'" <<uri_.m_password
            << "' h:'" <<uri_.m_host 
            << "' n:'" <<uri_.m_port 
            << "' f:'" <<uri_.m_path << "'";
    }

    Uri(const std::string& uri_) { 
        using  kalki::split2;
        std::string rest;

        std::tie(m_scheme, rest) = split2(uri_, "://");
        if(m_scheme.empty() || rest.empty()) {
            throw EXCEPTION("Malformed URI:" << uri_);
        }
        if(m_scheme == "file") {
            m_path = rest;
            return;
        }
        std::string first;
        std::string second;
        std::tie(first, second) = split2(rest, "@");
        if(first.empty()) {
            throw EXCEPTION("Malformed URI:" << uri_);            
        } else if(second.empty()) {
            // no username password present
            std::tie(m_host, m_port) = split2(first, ":");
        } else {
            // username and/or password present
            std::tie(m_user, m_password) = split2(first, ":");
            std::tie(m_host, m_port) = split2(second,":");
        }
        //std::cout << *this << std::endl;
        if(m_host.empty() || m_port.empty()) {
            throw EXCEPTION("Malformed URI:" << uri_);            
        }
        if(!m_password.empty() && m_user.empty()) {
            throw EXCEPTION("Malformed URI:" << uri_);            
        }
    }
};
}

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
