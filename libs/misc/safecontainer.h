#pragma once

template<typename Container>
class Safe {
public:
	class Lock {
	public:
		~Lock() { }
		Lock(Lock&& rhs_) : m_lock(std::move(rhs_.m_lock)), m_container(rhs_.m_container) {	}
		Container* operator->() { return &m_container; }
	private:
		friend class Safe; // use Safe::getLock() to create an instance
		Lock(Safe& safe_) : m_lock(safe_.m_mutex), m_container(safe_.m_container) { }
		
        unique_lock<mutex> m_lock;
		Container& m_container;
	};
	Lock getLock() {
		return Lock(*this);
	}
private:
	friend class Lock; 
	std::mutex m_mutex;
	Container m_container;
};

// to create a SafeContainer on the heap
template<typename Container>
std::unique_ptr<Safe<Container>> makeSafe() {
	return std::unique_ptr<Safe<Container>>(new Safe<Container>());
} 
