#pragma once

#include <chrono>		// 

namespace kalki {

class Stopwatch {
public:
	using Clock = std::chrono::steady_clock; // steady_clock 

	Stopwatch() : m_start(Clock::now()) {}
	std::chrono::nanoseconds elapsedNanos() const { 
		return (Clock::now() - m_start);
	}
	void restart() { m_start = Clock::now(); }
private:
	Clock::time_point m_start;
};

inline std::chrono::seconds to_seconds(unsigned num) { return std::chrono::seconds(num); }
inline std::chrono::milliseconds to_millis(unsigned num) { return std::chrono::milliseconds(num); }
inline std::chrono::microseconds to_micros(unsigned num) { return std::chrono::microseconds(num); }
inline std::chrono::nanoseconds to_nanos(unsigned num) { return std::chrono::nanoseconds(num); }

}