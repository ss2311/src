#pragma once

#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>
#include <utility>

namespace kalki {

template<typename Class>
class Thread : public Class, public std::thread {
public:
    template<typename... Args>
    Thread(std::chrono::milliseconds pollPeriod_, Args&&... args) : 
        Class(std::forward<Args>(args)...),
        // std::thread(&Thread::loop, this), // DO NOT start the thread at this point in time
        m_pollPeriod(pollPeriod_),
        m_run(true)
    { }
    void startThread() {
        m_run = true;
        this->start(); // MUST be implemented by class
        // now start the thread by re-assigning a new one to base class
        static_cast<std::thread&>(*this) = std::move(std::thread(&Thread::loop, this));
    }
    void stopThread() {
        this->stop(); // MUST be implemented by class
        m_run = false;
    }
private:
    void loop() {
        while(m_run) {
            std::this_thread::sleep_for(m_pollPeriod);
            this->poll(); // MUST be implemented by Class
        }
    }
    std::chrono::milliseconds m_pollPeriod;
    std::atomic<bool> m_run;
};
}