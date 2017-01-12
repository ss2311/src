#pragma once

#include <misc/safecontainer.h>
#include <feeds/orderbook.h>
#include <map>

namespace kalki {
class OrderBookMgr {
public:
    void createBook(Instrument::Id id_) {
        m_books.emplace(id_, makeSafe<OrderBook>(id_));
    }
    Safe<OrderBook>& getBook(Instrument::Id id_) {
        return *m_books.at(id_).get();
    }
private:
    std::unordered_map<Instrument::Id, std::unique_ptr<Safe<OrderBook>>> m_books;
    
};
} // namespace
