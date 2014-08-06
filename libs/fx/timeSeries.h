#pragma once
#include <misc/xtime.h>	// Time
#include <fx/price.h>	// price
#include <deque>		// deque
#include <misc/cBuffer.h>

namespace fx {

// struct for representing each time series entry
struct TsData
{
	Time m_time;
	Price m_price;
	
	// Less is declared THIS way for time ordering of TsData in containers
	friend bool operator<(const TsData& lhs_, const TsData& rhs_) { return lhs_.m_time < rhs_.m_time; }
};


/*
template<typename Type, unsigned Num>
class TsBuffer : public std::array<Type, Num>
{
public:
	TsBuffer() : m_begin(0), m_end(0)
	{}
	void push_back(const Type& data_)
	{
		this->at(m_end) = data_;
		m_end = (m_end + 1) % Num;

		if(m_end == m_begin)
			m_begin = (m_begin + 1 ) % Num;
	}
	unsigned begin() const { return m_begin; }
	unsigned end()   const { return m_end; }
private:
	unsigned m_begin;
	unsigned m_end;
};
*/

//////////////////////////////////////////////////////////////////////////
template<unsigned Num = 2000>
class TimeSeries : public tools::CircularBuffer<TsData, Num>
{
public:

	// ctor
	TimeSeries(unsigned size_) : m_maxSize(size_), m_sum(0) {}

	// get the most recent entry 
	const TsData getRecent() const { TsData data; this->back(data); return data; }

	// get the oldest entry 
	const TsData getOldest() const { TsData data; this->front(data); return data; }

	// insert new entry into TS
	void insert(const Price& price_, const Time time_ = Time::now())
	{
/*		if(size() == m_maxSize)
		{
			auto& data = getOldest();
			m_sum -= data.m_price.asDouble();	// remove the oldest entry from sum 
			pop_back();							// pop oldest entry
		}
*/		
		this->push_back({time_, price_});
		m_sum += price_.asDouble();	
	}

private:
	unsigned		m_maxSize;	// max num entries
	double			m_sum;		// sum of all entries
};
#if 0
//////////////////////////////////////////////////////////////////////////
//template<unsigned Num = 2000>
class TimeSeries : /*public tools::CircularBuffer<Price, Num>*/  public std::deque<TsData>
{
public:

	// ctor
	TimeSeries(unsigned size_) : m_maxSize(size_), m_sum(0) {}

	// get the most recent entry 
	const TsData& getRecent() const { return front(); }

	// get the oldest entry 
	const TsData& getOldest() const { return back(); }

	// insert new entry into TS
	void insert(const Price& price_, const Time time_ = Time::now())
	{
		if(size() == m_maxSize)
		{
			auto& data = getOldest();
			m_sum -= data.m_price.asDouble();	// remove the oldest entry from sum 
			pop_back();							// pop oldest entry
		}
		push_front({time_, price_});
		m_sum += price_.asDouble();	
	}

private:
	unsigned		m_maxSize;	// max num entries
	double			m_sum;		// sum of all entries
};
#endif

} // namespace fx
