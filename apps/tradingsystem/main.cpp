#include <iostream>
#include <config/config.h>
#include <logger/logger.h>
#include <strategy/strategy.h>
#include <strategy/stratController.h>
#include <misc/xtime.h>
#include <misc/xthread.h>
#include <cstdlib>

using namespace std;
using namespace logger;
using namespace tools;
using namespace kalki;

struct Dummy : public Strategy {
    ~Dummy() {}
    Dummy(StrategyController& controller_) : Strategy(controller_) {
        controller_.startTimer(*this, to_millis(1000), true, 0);
    }
    void onTimerUpdate(const Timer&) { cout << __FUNCTION__<< "() timer called" << endl; }
};

int main(int argc, char* argv[]) {
    if(argc < 2) {
        cerr << "Error: missing parameters" << endl;
        cerr << "usage: " << argv[0] << " <config.cfg>" << endl;
        return EXIT_FAILURE;
    }
    
    cout << "Reading config from " << argv[1] << endl;
    Config cfg(argv[1]);
    string logfile = cfg.getValue("Logfile","");
    Logger<> log;
    log.getSink().setFile(logfile);    
    cout << "Logger started ..." << endl;
    log.logINFO("Started ...");
    //// 
    Thread<StrategyController> sc1(to_millis(10));
    sc1.addStrategy(new Dummy(sc1));
    sc1.startThread();
    this_thread::sleep_for(to_seconds(5));
    sc1.stopThread();    
    sc1.join();
    return EXIT_SUCCESS;
}