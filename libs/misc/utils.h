#pragma once

#include <utility> // make_pair

namespace kalki {
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