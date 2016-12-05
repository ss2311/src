//
// 

#pragma once

#include <misc/exception.h>

namespace tools {

template<typename Type>
struct MemBlock
{
	Type data;
	int nextIdx;
	int selfIdx;
};


/*
template<typename Type>
static MemPool<Type,createMemPool(Type type, unsigned num)
{ 
	return new MemPool<Type, num>(); 
}
*/

template<typename Type, unsigned Num>
class MemPool : public std::array<MemBlock<Type>, Num> 
{
public:
	static const int INVALID_IDX = -1;


	MemPool()
	{
		for(unsigned i = 0; i < Num; i++)
		{
			this->at(i).nextIdx = i + 1;
			this->at(i).selfIdx = i;
		}	
		this->at(Num -1).nextIdx = INVALID_IDX;
	}

	Type& alloc()
	{
		if(m_headFree == INVALID_IDX)
			throw EXCEPTION("Out of Memory");

		auto& blk = this->at(m_headFree);
		m_headFree = blk.nextIdx;
		count++;
		return blk.data;
	}
	
	void free(Type& data)
	{
		auto& blk = static_cast<Type&>(data);
		blk.nextIdx = m_headFree;
		m_headFree = blk.selfIdx;
		count--;
	}
	unsigned numElems() const 
	{
		return count;
	}

private:
	int m_headFree = 0;
	int count = 0;
};

}

