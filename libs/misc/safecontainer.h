#pragma once

#include <mutex>

namespace kalki {

// Takes a container and adds a mutex to it and wraps a protective interface around it to make it thread safe
template<typename Container>
class Safe {
public:
	template<typename... Args>
	Safe(Args ... args) : m_container(args...) { }
    class Lock {
    public:
        ~Lock() { m_safe.m_mutex.unlock(); }   
        Container* operator->() { return &m_safe.m_container; }
    private:
        friend class Safe; // So only Safe can access my pvt ctor
        Lock(Safe& safe_) : m_safe(safe_) { m_safe.m_mutex.lock(); } // pvt ctor. Use Safe::getLock() to create instance
        Safe& m_safe;
    };  
    Lock getLock() { return Lock(*this); }   
private:
    friend class Lock; 
    friend class TestInspect;
    std::mutex m_mutex;
    Container m_container;
};

// to create a SafeContainer on the heap
template<typename Container>
std::unique_ptr<Safe<Container>> makeSafe() {
	return std::unique_ptr<Safe<Container>>(new Safe<Container>());
} 
} // namespace