#pragma once

class Order {
public:
    enum class Type : char {
        MARKET = 'M',
        LIMIT = 'L',
    };
    enum class Action : char {
        BUY = 'B',
        SELL = 'S'
    };
    enum class TimeInForce : char {
        DAY = 'D',
        GTC = 'G',
        FAK = 'A',
        FOK = 'O',
    };
    enum class State : char {
         INIT,
         SUBMITED,
         ACCEPTED,
         CANCELLED,
         PARTIALLY_FILLED,
         FILLED,
         // DONE -- TODO do we really need this ??
     };

Type getOrderType() const { return m_type; }
Action getAction() const { return m_action; }
//TimeInForce getTimeOnForce() const { return m_timeInForce; }

protected:
    Order(Type type_, Action action_, Instrument::Id instrumentId_, Quantity qty_) : 
        m_type(type_), m_action(action_), m_instrumentId(instrumentId_), m_qty(qty_) {
    }
    Type    m_type;
    Action  m_action;
    Instrument::Id m_instrumentId;
    Quantity m_qty;
};

class MarketOrder : Order {
public:
    MarketOrder createOrder(Order::Action action_, Instrument::Id id_, Quantity qty_) {
        return MarketOrder(action_, id_, qty_);
}
// protected:
    MarketOrder(Action action_, Instrument::Id id_, Quantity qty_) : Order(Order::Type::MARKET, action_, id_, qty_) {}    
};

class LimitOrder : Order {
public:
    LimitOrder createOrder(Order::Action action_, Instrument::Id id_, Quantity qty_) {
        return LimitOrder(action_, id_, qty_);
}
// protected:
    LimitOrder(Action action_, Instrument::Id id_, Quantity qty_) : Order(Order::Type::MARKET, action_, id_, qty_) {}    
};
