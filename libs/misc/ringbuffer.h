#pragma once

#include <atomic>	// atomic
#include <utility>	// std::move

//////////////////////////////////////////////////////////
// Single Reader Single Writer Ring Buffer
// read from head -- pop_front
// write to tail -- push_back
// TODO: Make this more efficient using non-consistent memory_ordering
template<typename Type, unsigned Num>
class RingBuffer {
public:
	static constexpr unsigned capacity() { return Num + 1; }
	RingBuffer() : m_read(0), m_write(0) { }
	bool isEmpty() const { return m_read == m_write; }
	bool isFull() const { return (m_write + 1) % capacity() == m_read ; }

class RingBufferWriter {
public:
    RingBufferWriter(RingBuffer& rb_) : m_rb(rb_) {}
    bool write(const Type& obj_) { return m_rb.write(obj_); } 
private:
    RingBuffer& m_rb;
};

class RingBufferReader {
public:
    RingBufferReader(RingBuffer& rb_) : m_rb(rb_) {}
    bool read(Type& obj_) { return m_rb.read(obj_); } 
private:
    RingBuffer& m_rb;
};

RingBufferReader getReader() { return RingBufferReader(*this); }
RingBufferWriter getWriter() { return RingBufferWriter(*this); }

private:
	bool write(const Type& obj) {
		if(isFull())
            return false;

        m_data[m_write] = std::move(obj); 
        m_write = (m_write + 1) % capacity(); 
        return true;
	}

	bool read(Type& obj) {
		if(isEmpty()) 
			return false;

		obj = std::move(m_data[m_read]);
		m_read = (m_read + 1) % capacity();
        return true;
	}
	// DATA MEMBERS
	std::atomic<unsigned> m_read; 	// read index (top)
	std::atomic<unsigned> m_write; 	// write index (tail)
	Type m_data[capacity()];
};
