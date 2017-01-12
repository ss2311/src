#include <boost/test/unit_test.hpp>
#include <boost/iostreams/device/mapped_file.hpp> // for mmap
#include <iostream>
#include <logger/logger.h>
#include <misc/xtime.h>
#include <misc/exception.h>
#include <feeds/feedCsv.h>

#include <sys/stat.h> 
#include <fcntl.h>

using namespace std;
using namespace kalki;


const char* PATH = "testData/AUDUSD.csv";
//const char* PATH = "/Users/saurabh/Downloads/ES_Sample/ES_Quotes.csv";
BOOST_AUTO_TEST_SUITE(ts_fileread)

BOOST_AUTO_TEST_CASE(test_ifstream)
{
    Stopwatch w;
    ifstream fs(PATH);
    std::string line;
    unsigned count = 0;
    while(std::getline(fs, line)){
        ++count;
    }
    auto dur = w.elapsed();
    cout << "ifstream count= " << count << " latency=" << dur.count() / 1000000.0 << " millis" << endl;    
}

BOOST_AUTO_TEST_CASE(test_mmap)
{
    Stopwatch w;
    boost::iostreams::mapped_file mmap(PATH, boost::iostreams::mapped_file::readonly);
    auto f = mmap.const_data();
    auto l = f + mmap.size();

    unsigned count = 0;
    while (f && f!=l)
        if ((f = static_cast<const char*>(memchr(f, '\n', l-f))))
            count++, f++;

    auto dur = w.elapsed();
    cout << "mmap count= " << count << " latency=" << dur.count() / 1000000.0 << " millis" << endl;    
}

BOOST_AUTO_TEST_CASE(test_read)
{
    Stopwatch w;
    static const auto BUFFER_SIZE = 16*1024;
    int fd = open(PATH, O_RDONLY);

    /* Advise the kernel of our access pattern.  */
    // posix_fadvise(fd, 0, 0, 1);  // FDADVICE_SEQUENTIAL

    char buf[BUFFER_SIZE + 1];
    unsigned count = 0;

    while(size_t bytes_read = read(fd, buf, BUFFER_SIZE))
    {
        if(bytes_read == (size_t)-1)
            throw EXCEPTION("read failed");
        if (!bytes_read)
            break;

        for(char *p = buf; (p = (char*) memchr(p, '\n', (buf + bytes_read) - p)); ++p)
            ++count;
    }
    auto dur = w.elapsed();
    cout << "read count= " << count << " latency=" << dur.count() / 1000000.0 << " millis" << endl;    
}
BOOST_AUTO_TEST_CASE(test_gtxcsv)
{
    Stopwatch w;
    FeedHandlerCsvGtx fhGtx("testData/AUDCAD2.csv", 4);
    fhGtx.startReading();
    auto dur = w.elapsed();
    cout << " latency=" << dur.count() / 1000000.0 << " millis" << endl;
}
BOOST_AUTO_TEST_SUITE_END()



