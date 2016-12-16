#pragma once

#include <chrono>
#include <strategy/position.h>

namespace kalki {

class StrategyController;
class Timer;

class Strategy {
public:
    Strategy(StrategyController& controller_) : m_controller(controller_) { }
    virtual ~Strategy() {}
    virtual void start() {}
    virtual void stop() {}
    // virtual void onTradingStateUpdate(const State&) {}
    // virtual void onConfigUpdate(const Config&) {}
    // virtual void onMarketUpdate(const Orderbook&) {}
    // virtual void onOrderUpdate(const Order&) {}
    virtual void onTimerUpdate(const Timer&) {}
    const Position& getPosition() const { return m_position; } 
protected:
    Position m_position;
    StrategyController& m_controller;
};

} // namespace