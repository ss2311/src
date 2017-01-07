#pragma once

#include <map>          // map
#include <limits>       // numeric_limits
#include <cmath>        // nan
#include <functional>   // greater
#include <iomanip>      // setprecision
#include <iostream>     // ostream


///////////////////////////////////////////////////////////////
template<typename T>
inline constexpr T power(T num_, unsigned exp_)
{
    return exp_ == 0 ? 1 : num_ * power(num_, exp_ - 1);
}

///////////////////////////////////////////////////////////////
class Instrument {
public:
    using Id = uint32_t;
    static const Id INVALID_ID = 0;
private:
    Id m_id;
    const char* m_name; 
};

///////////////////////////////////////////////////////////////
class Price {
public:
    static const unsigned PRECISION = 6;            // 6 digit of precision
    static const unsigned MULTIPLER = power(10, 6); // 6 digits of precision
    static const int64_t BAD_PX = std::numeric_limits<int64_t>::min(); 
    static constexpr double NAN_PX = std::nan(""); 

    Price(double px_) : m_value(scalePx(px_)) { }
    Price(int) = delete;    
    Price(const Price& px_) : m_value(px_.m_value) { }
    Price() : m_value(BAD_PX) { }
    operator int64_t() const { return m_value; }
    bool isBad() const { return m_value == BAD_PX; }
    double asDouble() const { return isBad() ? NAN_PX : static_cast<double>(m_value) / MULTIPLER; }

private:
    int64_t scalePx(double px) {
        double delta = px > 0 ? 0.5 : -0.5;
        return static_cast<int64_t>((px * MULTIPLER) + delta); 
    }

    int64_t m_value = BAD_PX;
};

///////////////////////////////////////////////////////////////
class Quantity {
public:
    Quantity(unsigned sz_ = 0) : m_value(sz_) {}
    operator uint32_t() const { return m_value; }
private:
    uint32_t m_value;
};

///////////////////////////////////////////////////////////////
// Plain old data struct so that it can be sent on wire or put in shared mem etc
struct OrderBook {
    enum class Side : char {
        BID = 'B',
        ASK = 'A'
    };
    static const unsigned MAX_LEVELS = 10;
    
    Instrument::Id m_instrumentId;
    uint16_t m_numBids;
    uint16_t m_numAsks;
    Price m_bidPx[MAX_LEVELS]; 
    Quantity m_bidQty[MAX_LEVELS];
    Price m_askPx[MAX_LEVELS]; 
    Quantity m_askQty[MAX_LEVELS];

    OrderBook(Instrument::Id id_) : m_instrumentId(id_), m_numBids(0), m_numAsks(0) { }

    // pretty printing
	friend std::ostream& operator<<(std::ostream& oss_, const OrderBook& ob_) {
        oss_ << "OrderBook for InstrumentId: " << ob_.m_instrumentId << std::endl;
        oss_ << std::fixed;
        oss_.precision(Price::PRECISION);
        for(unsigned i = 0; i < MAX_LEVELS; ++i) {
            //oss_ << i << " ";
            if(i < ob_.m_numBids)
                oss_ << std::left << ob_.m_bidPx[i].asDouble() << " x " << std::setw(10) << ob_.m_bidQty[i]; 
            if(i < ob_.m_numAsks)
                oss_ << ob_.m_askPx[i].asDouble() << " x " << ob_.m_askQty[i]; 
            oss_ << std::endl;
        }
        return oss_; 
    } 
};

///////////////////////////////////////////////////////////////
class OrderBookBuilder {
public:
    OrderBookBuilder(Instrument::Id id_) : m_instrumentId(id_) { }

    void addLevel(Price px_, Quantity sz_, OrderBook::Side side_) {
        if(side_ == OrderBook::Side::BID)
            addLevel(bidBook, px_, sz_);
        else
            addLevel(askBook, px_, sz_);
    }
    
    void removeLevel(Price px_, Quantity sz_, OrderBook::Side side_) {
        if(side_ == OrderBook::Side::BID)
            removeLevel(bidBook, px_, sz_);
        else
            removeLevel(askBook, px_, sz_);
    }
    
    void removeLevel(Price px_, Quantity sz_ = Quantity()) {
        if(bidBook.size()  && px_ <= bidBook.begin()->first) // is px <= best bid
            removeLevel(bidBook, px_, sz_);
        else
            removeLevel(askBook, px_, sz_);
    }

    OrderBook getOrderBook() {
        OrderBook ob(m_instrumentId);
        unsigned i = 0;
        for(auto it = bidBook.begin(); it != bidBook.end() && i < OrderBook::MAX_LEVELS; ++it, ++i ) {
            ob.m_bidPx[i] = it->first;
            ob.m_bidQty[i] = it->second;
        }
        ob.m_numBids = i;

        i = 0;
        for(auto it = askBook.begin(); it != askBook.end() && i < OrderBook::MAX_LEVELS; ++it, ++i ) {
            ob.m_askPx[i] = it->first;
            ob.m_askQty[i] = it->second;
        }
        ob.m_numAsks = i;
        return ob;
    }

private:
    template<typename Book>
    void addLevel(Book& book_, Price px_, Quantity sz_) {
        auto ret = book_.emplace(px_, sz_);
        if(false == ret.second) {       // px already exists
            ret.first->second += sz_;   // then update volume
        }
    }

    template<typename Book>
    void removeLevel(Book& book_, Price px_, Quantity sz_) {
        auto it = book_.find(px_);
        if(it != book_.end()) {     // px exists
            it->second -= sz_;      // update volume
            if(it->second <= 0 || sz_ == 0) // remove if px has no volume or if sz_ is zero
                book_.erase(it); 
        }
    }
    Instrument::Id m_instrumentId;
    // since we add/subtract Quantitys, so using signed int vs unsigned Quantity
    std::map<Price, int32_t, std::greater<Price>> bidBook; // best bid => highest bid 
    std::map<Price, int32_t> askBook; // best ask => lowest ask
};
