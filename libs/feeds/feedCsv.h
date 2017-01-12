#pragma once

#include <feeds/orderbook.h>
#include <misc/utils.h>     // KB

namespace kalki {
// static FeedHandler& createFeedHandler(std::string& uri_) {
//     Uri uri(uri_);
//     if(! uri.m_path.empty()) {
//         return * new FeedHandlerCsv(uri.m_path);
//     } else {
//         throw EXCEPTION("No feed handler found for: " << uri_);
//     }
// };

class FeedHandler {
    virtual ~FeedHandler() { }     
    virtual OrderBook getOrderBook() = 0;
};

class FeedHandlerCsv {
public:
    static const auto BUFFER_SIZE = 16*KB; 
    FeedHandlerCsv(const std::string& file_, Instrument::Id id_= Instrument::UNKNOWN);
    virtual void startReading();
    virtual OrderBook parseCsv(char* buff_, unsigned len_) = 0;
protected:
    char m_buffer[BUFFER_SIZE * 2 + 1]; // reassembly buffer
    unsigned m_nbytes = 0;              // num bytes in reassembly buffer
    Instrument::Id m_instrumentId;
    int m_fd;
};

// http://ratedata.gaincapital.com/2014/10%20October/AUD_CAD_Week1.zip
struct FeedHandlerCsvGtx : public FeedHandlerCsv {
    FeedHandlerCsvGtx(const std::string& file_, Instrument::Id id_= Instrument::UNKNOWN) : FeedHandlerCsv(file_, id_) {}
    OrderBook parseCsv(char* buff_, unsigned len_);
};

}