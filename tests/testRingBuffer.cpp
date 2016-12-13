#include <boost/test/unit_test.hpp>
#include <misc/ringbuffer.h>
#include <iostream>
#include <string>
#include <thread>
#include <misc/xtime.h>

using namespace std;

struct Data
{
    int32_t idata;
    string  sdata;
    double  ddata;

    friend ostream& operator<<(ostream& oss_, const Data& rhs_) {
        return oss_ << "idata=" << rhs_.idata << ", sdata=" << rhs_.sdata << ", ddata=" << rhs_.ddata;
    }
    friend bool operator ==(const Data& lhs, const Data& rhs)  {
        return  lhs.idata == rhs.idata && 
                lhs.sdata == rhs.sdata && 
                int(lhs.ddata *10000) == int(rhs.ddata*10000);
    }
};

///////////////////////////////////////////////////////////
//BOOST_AUTO_TEST_SUITE(rbuf)

BOOST_AUTO_TEST_CASE(test_basic)
{
    RingBuffer<Data, 6> rb;
    Data rdata, wdata;

    auto rbr = rb.getReader();
    auto rbw = rb.getWriter();

    // PRE-CONDITION
    BOOST_CHECK_EQUAL(rb.isEmpty(), true);
    BOOST_CHECK_EQUAL(rb.isFull(), false);
    // BOOST_CHECK_EQUAL(rb.len(), 0);
    BOOST_CHECK_EQUAL(rbr.read(rdata), false);
    
    // EXPERIMENT -- write
    wdata = {123, "HELLO", 100.123};
    BOOST_CHECK_EQUAL(rbw.write(wdata), true);
    
    // POST-CONDITION
    BOOST_CHECK_EQUAL(rb.isEmpty(), false);
    BOOST_CHECK_EQUAL(rb.isFull(), false);
    // BOOST_CHECK_EQUAL(rb.len(), 1);

    // EXPERIMENT - read 1
    BOOST_CHECK_EQUAL(rbr.read(rdata), true);

    // POSTCONDITION
    BOOST_CHECK_EQUAL(rdata, wdata);
    BOOST_CHECK_EQUAL(rb.isEmpty(), true);
    BOOST_CHECK_EQUAL(rb.isFull(), false);
    // BOOST_CHECK_EQUAL(rb.len(), 0);    
}

BOOST_AUTO_TEST_CASE(test_q_full)
{
    RingBuffer<Data, 6> rb;
    Data rdata, wdata;

    auto rbr = rb.getReader();
    auto rbw = rb.getWriter();

    for(int i = 1; i <= 6; ++i) {
        BOOST_CHECK_EQUAL(rbw.write({i+100, "HELLO" + to_string(i), (100 + i/1000.0)}), true);
    }
    // Writing the seventh one should fail
    BOOST_CHECK_EQUAL(rbw.write({107, "HELLO7", 100.07}), false);
    BOOST_CHECK_EQUAL(rb.isEmpty(), false);
    BOOST_CHECK_EQUAL(rb.isFull(), true);
    // BOOST_CHECK_EQUAL(rb.len(), 6);    

    for(int i = 1; i <= 6; ++i) {
        BOOST_CHECK_EQUAL(rbr.read(rdata), true);
        BOOST_CHECK_EQUAL(rdata, (Data{i+100, "HELLO" + to_string(i), (100 + i/1000.0)}));
    }
    // Reading the seventh one should fail
    BOOST_CHECK_EQUAL(rbr.read(rdata), false);
    BOOST_CHECK_EQUAL(rb.isEmpty(), true);
    BOOST_CHECK_EQUAL(rb.isFull(), false);
    // BOOST_CHECK_EQUAL(rb.len(), 0);        
}

BOOST_AUTO_TEST_CASE(test_rw_seq)
{
    RingBuffer<Data, 6> rb;
    Data rdata, wdata;

    auto rbr = rb.getReader();
    auto rbw = rb.getWriter();

    for(int i = 1; i <= 7; ++i) {
        BOOST_CHECK_EQUAL(rb.isEmpty(), true);
        // BOOST_CHECK_EQUAL(rb.len(), 0);
        BOOST_CHECK_EQUAL(rb.isFull(), false);
        BOOST_CHECK_EQUAL(rbw.write({i+100, "HELLO" + to_string(i), (100 + i/1000.0)}), true);
        BOOST_CHECK_EQUAL(rb.isEmpty(), false);
        // BOOST_CHECK_EQUAL(rb.len(), 1);
        BOOST_CHECK_EQUAL(rb.isFull(), false);
        BOOST_CHECK_EQUAL(rbr.read(rdata), true);
        BOOST_CHECK_EQUAL(rdata, (Data{i+100, "HELLO" + to_string(i), (100 + i/1000.0)}));
    }
}

BOOST_AUTO_TEST_CASE(test_rw_seq_batch)
{
    RingBuffer<Data, 7> rb;
    Data rdata, wdata;

    auto rbr = rb.getReader();
    auto rbw = rb.getWriter();

    BOOST_CHECK_EQUAL(rbw.write({101, "HELLO1", 100.01}), true);
    BOOST_CHECK_EQUAL(rbw.write({102, "HELLO2", 100.02}), true);
    BOOST_CHECK_EQUAL(rbw.write({103, "HELLO3", 100.03}), true);
    BOOST_CHECK_EQUAL(rbr.read(rdata), true);
    BOOST_CHECK_EQUAL(rdata, (Data{101, "HELLO1", 100.01}));
    BOOST_CHECK_EQUAL(rbr.read(rdata), true);
    BOOST_CHECK_EQUAL(rdata, (Data{102, "HELLO2", 100.02}));
    BOOST_CHECK_EQUAL(rbr.read(rdata), true);
    BOOST_CHECK_EQUAL(rdata, (Data{103, "HELLO3", 100.03}));
    BOOST_CHECK_EQUAL(rb.isEmpty(), true);
    BOOST_CHECK_EQUAL(rb.isFull(), false);

    BOOST_CHECK_EQUAL(rbw.write({101, "HELLO1", 100.01}), true);
    BOOST_CHECK_EQUAL(rbw.write({102, "HELLO2", 100.02}), true);
    BOOST_CHECK_EQUAL(rbw.write({103, "HELLO3", 100.03}), true);
    BOOST_CHECK_EQUAL(rbw.write({104, "HELLO4", 100.04}), true);
    BOOST_CHECK_EQUAL(rbw.write({105, "HELLO5", 100.05}), true);
    BOOST_CHECK_EQUAL(rbw.write({106, "HELLO6", 100.06}), true);
    BOOST_CHECK_EQUAL(rbw.write({107, "HELLO7", 100.07}), true);
    BOOST_CHECK_EQUAL(rb.isEmpty(), false);
    BOOST_CHECK_EQUAL(rb.isFull(), true);    

    BOOST_CHECK_EQUAL(rbr.read(rdata), true);
    BOOST_CHECK_EQUAL(rdata, (Data{101, "HELLO1", 100.01}));
    BOOST_CHECK_EQUAL(rbr.read(rdata), true);
    BOOST_CHECK_EQUAL(rdata, (Data{102, "HELLO2", 100.02}));
    BOOST_CHECK_EQUAL(rbr.read(rdata), true);
    BOOST_CHECK_EQUAL(rdata, (Data{103, "HELLO3", 100.03}));

    BOOST_CHECK_EQUAL(rbw.write({101, "HELLO1", 100.01}), true);
    BOOST_CHECK_EQUAL(rbw.write({102, "HELLO2", 100.02}), true);
    BOOST_CHECK_EQUAL(rbw.write({103, "HELLO3", 100.03}), true);

    BOOST_CHECK_EQUAL(rbr.read(rdata), true);
    BOOST_CHECK_EQUAL(rdata, (Data{104, "HELLO4", 100.04}));
    BOOST_CHECK_EQUAL(rbr.read(rdata), true);
    BOOST_CHECK_EQUAL(rdata, (Data{105, "HELLO5", 100.05}));
    BOOST_CHECK_EQUAL(rbr.read(rdata), true);
    BOOST_CHECK_EQUAL(rdata, (Data{106, "HELLO6", 100.06}));

    BOOST_CHECK_EQUAL(rbw.write({104, "HELLO4", 100.04}), true);

    BOOST_CHECK_EQUAL(rbr.read(rdata), true);
    BOOST_CHECK_EQUAL(rdata, (Data{107, "HELLO7", 100.07}));
    BOOST_CHECK_EQUAL(rbr.read(rdata), true);
    BOOST_CHECK_EQUAL(rdata, (Data{101, "HELLO1", 100.01}));
    BOOST_CHECK_EQUAL(rbr.read(rdata), true);
    BOOST_CHECK_EQUAL(rdata, (Data{102, "HELLO2", 100.02}));
    BOOST_CHECK_EQUAL(rbr.read(rdata), true);
    BOOST_CHECK_EQUAL(rdata, (Data{103, "HELLO3", 100.03}));
    BOOST_CHECK_EQUAL(rbr.read(rdata), true);
    BOOST_CHECK_EQUAL(rdata, (Data{104, "HELLO4", 100.04}));

    BOOST_CHECK_EQUAL(rb.isEmpty(), true);
}

#if 0
BOOST_AUTO_TEST_CASE(test_threads)
{
    RingBuffer<Data, 7> rb;
    Data rdata, wdata;

    auto rbr = rb.getReader();
    auto rbw = rb.getWriter();

    auto funcW = [&] () {
        for(int i = 0; i <20; ++i) {
            this_thread::sleep_for(chrono::milliseconds(400));
            while(rbw.write({i, "hello_" + to_string(i), 100 + i/1000.0 }) == false) {
                this_thread::sleep_for(chrono::milliseconds(100));
            }
        }        
        for(int i = 20; i <40; ++i) {
            this_thread::sleep_for(chrono::milliseconds(50));
            while(rbw.write({i, "hello_" + to_string(i), 100 + i/1000.0 }) == false)
                this_thread::sleep_for(chrono::milliseconds(100));
        } 
        while(false == rbw.write({-1, "", 0.0}));  // terminating sentinel     
    };

    auto funcR = [&] () {
        int i = 0;
        while(true) {
            this_thread::sleep_for(chrono::milliseconds(200));
            if(rbr.read(rdata)) {
                cout << rdata << endl;
                if(rdata.idata == -1)
                    return;
                BOOST_CHECK_EQUAL(rdata, (Data{i, "hello_" + to_string(i), 100 + i/1000.0 }));
                ++i;
            }
        }
    };

    thread t1(funcW);
    thread t2(funcR);

    t1.join();
    t2.join(); 
}

BOOST_AUTO_TEST_CASE(test_perf)
{
	RingBuffer<unsigned, 10> rb;

	auto rbr = rb.getReader();
	auto rbw = rb.getWriter();

	Timer timer;
	for(unsigned i = 0; i < 10*1000*1000; ++i) {
	    rbw.write(i);
		rbr.read(i);
	}
	auto dur = timer.elapsedNanos();
	cout << "elapsed=" << chrono::duration_cast<chrono::milliseconds>(dur).count() << " millis" << endl;

}
#endif
//BOOST_AUTO_TEST_SUITE_END()