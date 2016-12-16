#pragma once

#include <feeds/orderbook.h> // Size
// #include <unordered_map>
#include <vector>
#include <broker/order.h>

namespace kalki {

///////////////////////////////////////////////////////////////
class Position {
public:
    enum  Side {
        LONG = 1,
        SHORT = -1,
    };

    Position(int32_t pos_=0) : m_value(pos_) { }
    int32_t getRealizedPnL() const { return 0;} // TODO: Need access to feed / mktUpdate / latest px for this instrument
    int32_t getUnrealizedPnL() const {return 0;}
    Instrument::Id getInstrument() const { return m_instrumentId; }
    // void enter(Quantity , Side side_) { }
    // void exit(Quantity , Side side_) {}
    int32_t getOpenPosition() const {return 0;}
    Price getOpenPositionUSD() const {return Price(0.0); }
    // const std::vector<Order>& getOpenOrders() const {  }
    void updateOrder(const Order&) {}
private:
    Instrument::Id m_instrumentId;
    int32_t m_value;
};

// Portfolio consists of positions in multiple instruments
// class Portfolio {
// public:
//     // https://herbsutter.com/2013/05/30/gotw-90-solution-factories/
// 	boost::optional<Position> getPosition(Instrument::Id) { 
// 		try { 
// 			return std::move(m_positions.at(Id)); 
// 		} catch(...) {
// 			return {}; //
// 		}
// 	}
// private:
// 	std::unordered_map<Instrument::Id, Position> m_positions;
// };
} // namespace
