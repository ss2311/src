#pragma once
#include <array>
#include <cassert>

namespace tools {


//////////////////////////////////////////////////////////
// TODO:
// Using array as it is fast and can be instatiated in 
// shared memory
template<typename Type, unsigned Num>
class CircularBuffer : public std::array<Type,Num+1> // one extra slot 
{
public:
	struct Iterator
	{
		Iterator(const CircularBuffer& cb_, unsigned idx_) : m_cb(cb_), m_currIdx(idx_) {}

		const CircularBuffer& m_cb;
		unsigned m_currIdx;
	};

	//////////////////////////////////////////////////////
	CircularBuffer()  : m_rdIdx(0), m_wrIdx(0)
	{
		assert(this->size() == Num + 1);
	}

	//////////////////////////////////////////////////////
	bool front(Type& data_) const
	{
		if(isEmpty()) 
			return false;

		data_ = this->at(m_rdIdx);
		return true;
	}

	//////////////////////////////////////////////////////
	bool back(Type& data_) const
	{
		if(isEmpty()) 
			return false;

		data_ = this->at((m_wrIdx - 1) % this->size());
		return true;
	}

	//////////////////////////////////////////////////////
	bool pop_front(Type& data_)
	{
		bool ret = front(data_);
		if(ret)
			incReadIdx();
		return ret;
	}

	//////////////////////////////////////////////////////
	bool read(Type& data_) { return pop_front(data_); }

	//////////////////////////////////////////////////////
	void push_back(const Type& data_)
	{
		this->at(m_wrIdx) = data_;
		incWriteIdx();

		// if read and write idx become same, increment read idx
		if(m_wrIdx == m_rdIdx)
			incReadIdx();
	}

	//////////////////////////////////////////////////////
	void write(const Type& data_) { push_back(data_); }

	//////////////////////////////////////////////////////
	bool isEmpty() const  { return m_rdIdx == m_wrIdx; }

	//////////////////////////////////////////////////////
	bool isFull() const  { return (m_wrIdx + 1) % this->size() == m_rdIdx; }

	//////////////////////////////////////////////////////
	// DO NOT use this func to determine how many read() you shud do
	// EITHER check the return value of read 
	// OR call isEmpty() before calling read 
	unsigned numElems() const  { return  m_rdIdx <= m_wrIdx ? m_wrIdx - m_rdIdx : m_wrIdx + this->size() - m_rdIdx; }

protected:
	//////////////////////////////////////////////////////
	void incReadIdx()  { m_rdIdx = (m_rdIdx + 1) % this->size(); }
	void incWriteIdx() { m_wrIdx = (m_wrIdx + 1) % this->size(); }
	friend Iterator;
 
	unsigned m_rdIdx;	// read index
	unsigned m_wrIdx;	// write index
};

/////////////////////////////////////////////////////////////////
struct CBuffReader
{
	
};

struct CBuffWriter
{
};


} // namespace tools

