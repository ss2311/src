#pragma once

#include <utility> // make_pair

namespace kalki {

static const unsigned KB = 1024; 
static const unsigned MB = KB * KB;
static const unsigned GB = KB * MB;

static const unsigned KILO = 1000; 
static const unsigned MEGA = KILO * KILO;
static const unsigned GIGA = MEGA * MEGA;

inline std::pair<std::string, std::string> split2(const std::string& str_, const std::string delim_) {
    using namespace std;

    string str1, str2;
    auto pos = str_.find_first_of(delim_);
    str1 = str_.substr(0, pos);
    if(pos != string::npos)
        str2 = str_.substr(pos + delim_.length());
    return make_pair(str1, str2); 
}

}