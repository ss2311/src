#include <iostream>
#include <cstdlib>
#include <config/config.h>
#include <logger/logger.h>
#include <strategy/strategy.h>
#include <strategy/stratController.h>
#include <feeds/orderbookMgr.h>

using namespace std;
using namespace logger;
using namespace tools;
using namespace kalki;

int main(int argc, char* argv[]) {
    if(argc < 2) {
        cerr << "Error: missing parameters" << endl;
        cerr << "usage: " << argv[0] << " <config.cfg>" << endl;
        return EXIT_FAILURE;
    }
    cout << "Reading config from " << argv[1] << endl;
    Config cfg(argv[1]);
    string logfile = cfg.getValue("LogFile","");
    Logger<> log;
    log.getSink().setFile(logfile);    
    cout << "Logger started ..." << endl;
    log.logINFO("Started ...");
    auto syms = cfg.getVector<string>("Instruments");
    OrderBookMgr bookMgr;
    for(unsigned i = 0; i < syms.size(); ++i) {
        log.logINFO("Created order book for sym: " << syms[i]);
        bookMgr.createBook(i);
    }
    return EXIT_SUCCESS;
}