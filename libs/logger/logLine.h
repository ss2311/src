#pragma once

namespace logger {

///////////////////////////////////////////////////////////////////////////////
// pretty print timespec timestamp
inline std::ostream& operator << (std::ostream& os_, const timespec& tspec_)
{
    tm * ptm = gmtime(&tspec_.tv_sec);
    char buff[128];
    strftime(buff, sizeof(buff), "%F|%Z|%X.", ptm);
    return os_ << buff << std::setfill('0') << std::setw(6)  << (tspec_.tv_nsec/1000);
}

///////////////////////////////////////////////////////////////////////////////
enum class LogLevel { DBG, INF, WRN, ERR, DIE};
inline std::ostream& operator << (std::ostream& os_, const LogLevel& lvl_)
{
    const char* label = "UNKNOWN";
    switch(lvl_)
    {
        case LogLevel::DBG: label = "DBG"; break;
        case LogLevel::INF: label = "INF"; break;
        case LogLevel::WRN: label = "WRN"; break;
        case LogLevel::ERR: label = "ERR"; break;
        case LogLevel::DIE: label = "DIE"; break;
    }
    return os_ << label;
}
    
    
///////////////////////////////////////////////////////////////////////////////
// LogItem represent each item in the LogLine
struct LogItemBase
{
    unsigned m_size;
    unsigned getSize() const { return m_size; }
    
    LogItemBase(unsigned sz_) : m_size(sz_) {}
    
    virtual std::ostream& show(std::ostream& os_) const { return os_ << "No show operator defined"; }private:
    
protected:
    virtual ~LogItemBase() {} // NOT to be instantiated on stack
};

template <typename T>
struct LogItem : LogItemBase
{
    T m_data;
    LogItem(const T& data_) : LogItemBase(sizeof(*this)), m_data(data_) {}
    // this ctor is ONLY here to help compile write() below. It shud never be called
    LogItem(const T& data_, unsigned) : LogItemBase(sizeof(*this)), m_data(data_) { assert(false);}
    std::ostream& show(std::ostream& os_) const { return os_ << m_data; }
};

template <>
struct LogItem<const char*> : LogItemBase
{
    char m_data[];
    LogItem(const char* data_, unsigned len_) : LogItemBase(sizeof(*this) + len_)
    { strcpy(m_data, data_); }
    LogItem(const char* data_) : LogItem(data_, strlen(data_) + 1) {}
    LogItem(const LogItem& rhs_) : LogItem(rhs_.m_data, strlen(rhs_.m_data) + 1) {}
    std::ostream& show(std::ostream& os_) const { return os_ << m_data; }
};

///////////////////////////////////////////////////////////////////////////////
const unsigned BUFF_MAX = 1024*2; // MAX size of each log line
const char SNIP[] = "[snip]";
const unsigned MIN_BUFF_SIZE = sizeof(SNIP) + sizeof(LogItem<const char*>) + 8;
static_assert(BUFF_MAX > MIN_BUFF_SIZE, "BUFF_MAX > MIN_BUFF_SIZE failed");

struct LogLine
{
    typedef std::deque<LogLine> Queue;
    
    ///////////////////////////////////////////////////////////////////////////
    // ctor
    LogLine(LogLevel lvl_, const char* file_, unsigned line_) :
    m_numItems(0), m_curr(m_buffer), m_level(lvl_) , m_file(file_), m_line(line_), m_snipped(false)
    {
        clock_gettime(CLOCK_REALTIME, &m_tspec);
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // for logging most data structs (int, double, structs, etc)
    template <typename T>
    LogLine& operator << (const T& data_)
    {
        return write(data_);
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // specialized version for logging char[]
    // refer: http://cplusplus.bordoon.com/specializeForCharacterArrays.html
    template<std::size_t LEN>
    LogLine& operator << (const char (&str_)[LEN])
    {
        return write(static_cast<const char*>(str_), LEN);
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // specialized version for logging std::string
    LogLine& operator << (const std::string& str_)
    {
        return write(str_.c_str(), str_.size()+1);
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // pretty printing of logline
    friend std::ostream& operator << (std::ostream& os_, const LogLine& line_)
    {
        os_ << line_.m_tspec << '|' << line_.m_level << '|';
        const char* ptr = line_.m_buffer;
        unsigned num = line_.m_numItems;
        while(num--)
        {
            const LogItemBase& ref = *reinterpret_cast<const LogItemBase*>(ptr);
            ref.show(os_);
            ptr = ptr + ref.getSize();
        }
        os_ << '|' << line_.m_file << '|' << line_.m_line << '|';
        return os_;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    LogLevel getLogLevel() const { return m_level; }
    
private:
    
    ///////////////////////////////////////////////////////////////////////////
    // Common writing routine. Also handles cases where len is different from sizeof()
    // example - char* strings
    template <typename T>
    LogLine& write(const T& data_, unsigned len_=0)
    {
        if( (m_curr + sizeof(LogItem<T>) + len_) < (m_buffer + BUFF_MAX - MIN_BUFF_SIZE))
        {
            LogItem<T> * logItem = nullptr;
            if(len_)
                logItem = new(m_curr) LogItem<T>(data_, len_);
            else
                logItem = new(m_curr) LogItem<T>(data_);
            
            m_curr+= logItem->getSize();
            m_numItems++;
            return *this;
        }
        return writeSnip();
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // Writes a marker in the log that log line was longer than the underlying
    // buffer hence, full log is NOT displayed
    LogLine& writeSnip()
    {
        if(m_snipped)
            return *this;
        m_snipped = true; // remember we added the marker - so dont add it again
        return *this << SNIP;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // DATA MEMBERS
    
    unsigned        m_numItems; // number of items in this LogLine
    char*           m_curr;     // pointer to current location
    LogLevel        m_level;    // LogLevel
    timespec        m_tspec;    // timestamp of this LogLine
    const char*     m_file;     // __FILENAME__
    unsigned        m_line;     // __LINE__
    bool            m_snipped;  // log line full -- no more insertion allowed
    char            m_buffer[BUFF_MAX];   // underlying buffer where logline is stored
    //    TODO std::array<char, BUFF_MAX> m_buffer;
};
    
///////////////////////////////////////////////////////////////////////////////
//
struct LogXLine : public std::ostringstream
{
    struct Queue : public std::deque<std::string>
    {
        void push_back(const std::ostringstream& oss_) { deque::push_back(oss_.str()); }
    };
    
    LogXLine(LogLevel level_) : m_level(level_) {}
    LogLevel getLogLevel() const { return m_level; }
  
    LogLevel m_level;
};

    
} // namespace logger
