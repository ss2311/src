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
	//////////////////////////////////////////////////////
	CircularBuffer()  : m_rdIdx(0), m_wrIdx(0)
	{
		assert(this->size() == Num + 1);
	}

	//////////////////////////////////////////////////////
	bool read(Type& data_)
	{
		if(isEmpty()) 
			return false;

		data_ = this->at(m_rdIdx);
		incReadIdx();
		return true;
	}

	//////////////////////////////////////////////////////
	void write(const Type& data_)
	{
		this->at(m_wrIdx) = data_;

		// increment write idx and if becomes same as read
		// then increment read idx too
		incWriteIdx();
		if(m_wrIdx == m_rdIdx)
			incReadIdx();
	}

	//////////////////////////////////////////////////////
	bool isEmpty() const  { return m_rdIdx == m_wrIdx; }

	//////////////////////////////////////////////////////
	bool isFull() const  { return (m_wrIdx + 1) % this->size() == m_rdIdx; }

	//////////////////////////////////////////////////////
	// DO NOT use this func to determine how many read() you shud do
	// EITHER check the return value of read 
	// OR call isEmpty() before calling read 
	unsigned numElems() const  { return  m_rdIdx <= m_wrIdx ? m_wrIdx - m_rdIdx : m_wrIdx + this->size() - m_rdIdx; }

private:
	//////////////////////////////////////////////////////
	void incReadIdx()  { m_rdIdx = (m_rdIdx + 1) % this->size(); }
	void incWriteIdx() { m_wrIdx = (m_wrIdx + 1) % this->size(); }
 
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

