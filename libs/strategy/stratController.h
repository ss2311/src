#pragma once 

#include <vector>
#include <set>      // set
#include <utility>  // unique_ptr
#include <chrono>
#include <strategy/strategy.h>

namespace kalki {

struct Timer {
    Strategy& strategy; 
    std::chrono::steady_clock::time_point tp;       // expiration time
    std::chrono::milliseconds ms;    // duration
    bool recurring;
    uint64_t userData;          // user data

    friend bool operator<(const Timer& lhs_, const Timer& rhs_) { return lhs_.ms < rhs_.ms; }
};

class StrategyController {
public:

    void addStrategy(Strategy* strat_) {
        m_strategies.push_back(strat_);
    }  
    void start() { for(auto s : m_strategies) s->start(); }
    void stop() { for(auto s : m_strategies) s->stop(); }
    // void onTradingStateUpdate(const State&) {}
    // void onConfigUpdate(const Config& cfg_) { for(auto s : m_strategies) s->onConfigUpdate(cfg_); }
    // void onMarketUpdate(const Orderbook& ob_) { for(auto s : m_strategies) s->onConfigUpdate(ob_); }
    // void onOrderUpdate(const Order& odr_) { for(auto s : m_strategies) s->onOrderUpdate(odr_); }
    // const Position& getPosition() const { return m_position; } 

    ////
    const Timer& startTimer(Strategy& strategy_, std::chrono::milliseconds, bool recurring, uint64_t usrdata);
    void poll();
private:
    // std::vector<std::unique_ptr<Strategy>> m_strategies;
    std::vector<Strategy*> m_strategies;
    std::multiset<Timer> m_timers;
};

} // namespace