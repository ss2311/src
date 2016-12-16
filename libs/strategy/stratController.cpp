#include <strategy/stratController.h>

using namespace kalki;
using namespace std;

using Clock = std::chrono::steady_clock;

const Timer& StrategyController::startTimer(Strategy& strategy_, chrono::milliseconds ms_, bool recurring_, uint64_t usrdata_) {
    return *m_timers.insert({strategy_, Clock::now() + ms_, ms_, recurring_, usrdata_});
}

void StrategyController::poll() {
    // check for timer expiry
    auto now = Clock::now();
    for (auto it = m_timers.begin(); it != m_timers.end(); ) {
        if(now >= it->tp) {
            it->strategy.onTimerUpdate(*it);
            if(it->recurring) {
                startTimer(it->strategy, it->ms, it->recurring, it->userData);
            }
            it = m_timers.erase(it); // remove the expired timer
        } else {
            break; // since timers are sorted by tp, no subsequent one would have expired
        }
    }
}