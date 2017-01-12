#include <feeds/feedCsv.h>
#include <fcntl.h>          // O_RDONLY
#include <unistd.h>         // read()
#include <cstring>          // memcpy
#include <misc/exception.h> // EXCEPTION
#include <iostream>

using namespace std;
using namespace kalki;


FeedHandlerCsv::FeedHandlerCsv(const std::string& file_, Instrument::Id id_) : 
    m_nbytes(0),
    m_instrumentId(id_),
    m_fd(open(file_.c_str(), O_RDONLY)) 
{ }

void FeedHandlerCsv::startReading() { 
    /* Advise the kernel of our access pattern.  */
    // posix_fadvise(fd, 0, 0, 1);  // FDADVICE_SEQUENTIAL

    char buf[BUFFER_SIZE + 1];

    while(size_t bytes_read = read(m_fd, buf, BUFFER_SIZE))
    {
        if(bytes_read == (size_t)-1)
            throw EXCEPTION("read failed");
        if (!bytes_read)
            break;

        for(char *p = buf; p; ++p)  {  
            char* q = (char*) memchr(p, '\n', (buf + bytes_read) - p); // find next EOL
            if(!q) {
                // no EOLs in read buffer. So copy the partial msg into reassembly buffer
                unsigned newbytes = (buf + bytes_read) - p;
                memcpy(m_buffer + m_nbytes, p, newbytes);
                m_nbytes += newbytes;
                break;
            } 
            if(m_nbytes) { // Do we have a partial msg in reassembly buffer ?
                // append read bytes into reassembly buffer to construct a full msg
                memcpy((char*)m_buffer + m_nbytes, p, (int)(q - p));
                parseCsv(m_buffer, m_nbytes + q - p);
                m_nbytes = 0;
            } else {
                parseCsv(p, q - p);
            }
            p = q;
        }
    }
}

OrderBook FeedHandlerCsvGtx::parseCsv(char* buff_, unsigned len_) {
    //csv format: lTid,cDealable,CurrencyPair,RateDateTime,RateBid,RateAsk
    char * p = buff_;
    p = (char*) memchr(p, ',', (buff_ + len_) - p); // find 1st comma
    p = (char*) memchr(p+1, ',', (buff_ + len_) - p); // find 2nd comma
    char* sym = p+1;
    p = (char*) memchr(sym, ',', (buff_ + len_) - p); // find 3rd comma
    *p = 0;
    char* dt = p+1;
    p = (char*) memchr(dt, ',', (buff_ + len_) - p); // find 4th comma
    *p = 0;
    char* bid = p+1;        
    p = (char*) memchr(bid, ',', (buff_ + len_) - p); // find 5th comma
    *p = 0;
    char* ask = p+1;
    buff_[len_] = 0;
    auto id = m_instrumentId; 
    if(Instrument::UNKNOWN == id) { // TODO : // if m_instrumentId is UNKNOWN then lookup "sym"
        // id = getInstrumentId(sym); // map the read symbol to InstrumentId
    }
    OrderBook ob(id); 
    ob.m_numBids = 1;
    ob.m_bidPx[0] = Price(std::atof(bid));
    ob.m_bidQty[0] = 1;
    ob.m_numAsks = 1;
    ob.m_askPx[0] = Price(std::atof(ask));;
    ob.m_askQty[0] = 1;
    cout << ob << endl;
    return ob;
}