#pragma once

#include <sstream>
#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <memory>
#include <deque>
#include <thread>
#include <mutex>
#include <chrono>
#include <type_traits>
#include <logger/logLine.h>

// TODO
// - appname in logs
// what if deque is full

namespace logger {

///////////////////////////////////////////////////////////////////////////////
// The sink which does the actual job of writing the logs into a file / cout
// Sink runs in its own separate thread
template<typename T>
class LogSink
{
public:
    
    ///////////////////////////////////////////////////////////////////////////
    // Singleton Common LogSink
    static LogSink& getGlobalSink()
    {
        static LogSink sink;
        return sink;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // ctor
    LogSink() : m_run(true), m_stream(&std::cout), m_thread([this]{ run(); })  { }
 
    ///////////////////////////////////////////////////////////////////////////
    // dtor
    // deactivate the run loop and wait for the thread to finish
    // this ensures that we drain the Q before dying - so all pending logs are
    // written to console / file
    ~LogSink()
    {
        m_run = false;
        m_thread.join();
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // sets the log file - to log into a file vs stdout
    // NOTE: there is no lock here - bcoz the assumption is that users will
    // set the file at the beginnig and not later
    void setFile(const std::string& filename_)
    {
        m_file.open(filename_.c_str());
        if(false == m_file.is_open())
            throw std::runtime_error("Cannot open filename " + filename_);

        std::cout << "Logging into file: " + filename_ << std::endl;
        m_stream = &m_file;
    }

    ///////////////////////////////////////////////////////////////////////////
    // to log into an already open stream
    void setStream(std::ostream& os_) { m_stream = &os_; }
    
    ///////////////////////////////////////////////////////////////////////////
    // Called by Logger to write a logline into the sink
    void writeToQ(const T& line_)
    {
        // lock before updating Q
        std::lock_guard<std::mutex> lock(m_mutex);
        m_deque.push_back(line_);
    }

    ///////////////////////////////////////////////////////////////////////////
    // run loop for the LogSink thread
    void run()
    {
        const std::chrono::milliseconds dura(50);
        while(true == m_run || false == m_deque.empty())
        {
            if(true == m_deque.empty())
            {
                // since Q is empty, therefore sleep for some time
                std::this_thread::sleep_for(dura);
                continue;
            }
            
            const auto & line = m_deque.front();
            *m_stream << line << std::endl;
            
            // lock before updating Q
            std::lock_guard<std::mutex> lock(m_mutex);
            m_deque.pop_front();
        }
    }
    
private:
    
    bool            m_run;      // is actively running
    std::ofstream   m_file;     // file where to write logs
    std::ostream*   m_stream;   // pointer to stream (cout or file)
    std::mutex      m_mutex;    // mutex for critical region
    typename T::Queue m_deque;  // Q for holding log msgs
    std::thread     m_thread;   // Sink thread - MUST be created after other members
};
    
///////////////////////////////////////////////////////////////////////////////
// This is what the users use for writing logs
// The Logger only packs the logs into a LogLine and then delegates the job of
// actual writing to the LogSink (which is running in its own thread)
template<typename T=LogLine>
class Logger
{
public:
    
    ///////////////////////////////////////////////////////////////////////////
    // ctor
    // Default sink is the common global sink
    // Default log level is INFO
    Logger(LogSink<T>& sink_ = LogSink<T>::getGlobalSink(), LogLevel lvl_ = LogLevel::INF) :
        m_sink(sink_),
        m_level(lvl_)
    {}

    ///////////////////////////////////////////////////////////////////////////
    LogLevel getLevel() const { return m_level;}
    
    ///////////////////////////////////////////////////////////////////////////
    void setLevel(LogLevel lvl_) {  if(isValid(lvl_)) m_level = lvl_; }

    ///////////////////////////////////////////////////////////////////////////
    void setLevel(const std::string& lvl_)
    {
        if("DBG" == lvl_) return setLevel(LogLevel::DBG);
        if("INF" == lvl_) return setLevel(LogLevel::INF);
        if("WRN" == lvl_) return setLevel(LogLevel::WRN);
        if("ERR" == lvl_) return setLevel(LogLevel::ERR);
        if("DIE" == lvl_) return setLevel(LogLevel::DIE);
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // Is this a valid logLevel
    bool isValid(LogLevel lvl_) { return lvl_ >= LogLevel::DBG && lvl_ <= LogLevel::DIE; }
	
    ///////////////////////////////////////////////////////////////////////////
    // Checks if the given LogLevel will log (with the current LogLevel)
    bool canLog(LogLevel lvl_) { return lvl_ >= m_level; }

    ///////////////////////////////////////////////////////////////////////////
    // Return the sink associated with this Logger
    LogSink<T>& getSink() { return m_sink; }
    
    ///////////////////////////////////////////////////////////////////////////
    // The main logging function
    void logXXX(const T& txt_)
    {
        if(false == canLog(txt_.getLogLevel()))
            return;
        
        m_sink.writeToQ(txt_);
    }
    
private:
    LogSink<T>& m_sink;     // LogSink
	LogLevel    m_level;    // currently configured log level
};

///////////////////////////////////////////////////////////////////////////
// Useful MACROS to be used with Logger.
// Ex.
//  Logger logger
//  logger.logINFO("Hello World." << " Current year is" << 2014);
    
// NOTE: __FILENAME__ is defined in the CMakeLists.txt as abbreviated path to file
#define logDBG(x)  logXXX(LogLine(LogLevel::DBG, __FILENAME__, __LINE__) << x )
#define logINFO(x) logXXX(LogLine(LogLevel::INF, __FILENAME__, __LINE__) << x )
#define logWARN(x) logXXX(LogLine(LogLevel::WRN, __FILENAME__, __LINE__) << x )
#define logERR(x)  logXXX(LogLine(LogLevel::ERR, __FILENAME__, __LINE__) << x )
#define logDIE(x)  logXXX(LogLine(LogLevel::DIE, __FILENAME__, __LINE__) << x )

#define logXDBG(x)  logXXX(LogXLine(LogLevel::DBG) << x << '|' << __FILENAME__ << '|' << __LINE__ << '|')
#define logXINFO(x) logXXX(LogXLine(LogLevel::INF) << x << '|' << __FILENAME__ << '|' << __LINE__ << '|')
#define logXWARN(x) logXXX(LogXLine(LogLevel::WRN) << x << '|' << __FILENAME__ << '|' << __LINE__ << '|')
#define logXERR(x)  logXXX(LogXLine(LogLevel::ERR) << x << '|' << __FILENAME__ << '|' << __LINE__ << '|')
#define logXDIE(x)  logXXX(LogXLine(LogLevel::DIE) << x << '|' << __FILENAME__ << '|' << __LINE__ << '|')
    
    
}


