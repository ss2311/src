#pragma once

#include <chrono>		// 

class Timer {
public:
	using Clock = std::chrono::steady_clock; // system_clock

	Timer() : m_start(Clock::now()) {}
	std::chrono::nanoseconds elapsedNanos() const { 
		return (Clock::now() - m_start);
	}
	void restart() { m_start = Clock::now(); }
private:
	Clock::time_point m_start;
};
