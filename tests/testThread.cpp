#include <boost/test/unit_test.hpp>
#include <misc/xthread.h>
#include <misc/xtime.h>
#include <iostream>
#include <string>

using namespace std;
using namespace kalki;

#if 0
#define VAL(x) #x"=" << x << ", " 

struct MyClass{
    MyClass(int x, float y, string s) {
        cout << "Init completed:" << VAL(x) << VAL(y) << VAL(s) << endl;
    }
    void poll() {
        cout << "Invoked " << __FUNCTION__ << endl;
    }
};

BOOST_AUTO_TEST_CASE(test_xthread)
{
    Thread<MyClass> tclass(to_millis(500), 10, 50.3, "hello");
    this_thread::sleep_for(to_seconds(5));
    tclass.stop();
    tclass.join();
}
#endif