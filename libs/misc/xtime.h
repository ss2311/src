#pragma once

#include <time.h>		// clock_gettime
#include <stdint.h>		// uint64_t
#include <string>		// string

//////////////////////////////////////////////////////////////////////////////////////
// Enacpsulates time
class Time
{
public:
	static uint64_t secondToMilli(uint64_t sec_) { return sec_ * 1000; }
	static uint64_t secondToMicro(uint64_t sec_) { return secondToMilli(sec_) * 1000; } 
	static uint64_t secondToNano(uint64_t sec_)  { return secondToMicro(sec_) * 1000; } 

	// returns time in nanosecond
	static uint64_t now(clockid_t clk_id_ = CLOCK_MONOTONIC) 
	{
		timespec ttime;
		clock_gettime(clk_id_, &ttime);
		return secondToNano(ttime.tv_sec) + ttime.tv_nsec;
	}

	Time(uint64_t time_ = now()) : m_time(time_) {} 
	uint64_t asNano() const { return m_time; } 
	uint64_t asMicro() const { return asNano() / 1000; } 
	uint64_t asMilli() const { return asMicro() / 1000; } 
	uint64_t asSec() const { return asMilli() / 1000; }
	double  asDouble() const { return static_cast<double>(m_time) / 1000.0*1000.0*1000.0; } 

	// Logical Operators
	friend bool operator < (const Time& lhs_, const Time& rhs_) { return lhs_.m_time < rhs_.m_time; }
	friend bool operator > (const Time& lhs_, const Time& rhs_) { return lhs_.m_time > rhs_.m_time; }
	friend bool operator == (const Time& lhs_, const Time& rhs_) { return lhs_.m_time == rhs_.m_time; }

	// This works the best with CLOCK_REALTIME
	std::string asUTC() 
	{
		time_t ttime = asSec();
		tm  tmtime;
		gmtime_r(&ttime, &tmtime);
		char buf[64];
		asctime_r(&tmtime, buf);
		std::string str(buf);
		str.pop_back(); // drop the \n at the end
		return str;
	}

private:
	uint64_t m_time; // stored in nanos
};

