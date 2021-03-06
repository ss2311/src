//
//  config.h
//  
//
//  Created by SAURABH SRIVASTAVA on 6/18/14.
//
//

#pragma once

#include <unordered_map>
#include <string>
#include <fstream>
#include <vector>
#include <misc/exception.h>
#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>

namespace tools {

////////////////////////////////////////////////////////////////////////////
//
struct Config : public std::unordered_map<std::string, std::string>
{
    ////////////////////////////////////////////////////////////////////////////
    Config(const std::string& filename_ = "")
    {
        if(filename_.empty())
            return;
        
        std::fstream fs(filename_);
        if(false == fs.is_open())
            throw EXCEPTION("Unable to open filename="<<filename_);
        readFile(fs);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    void readFile(std::istream& fs_)
    {
        std::string line;
        while(std::getline(fs_, line))
        {
            boost::trim(line);
            
            if(0 == line.length())  // skip empty lines
                continue;
            
            if('#' == line[0])      // skip comments
                continue;
            
            unsigned pos = line.find('=');
            if(pos > line.length())
                throw EXCEPTION("Key Value delimiter not found");
            std::string key = line.substr(0, pos);
            std::string val = line.substr(pos+1);
            boost::trim(key);
            boost::trim(val);
            
            if(false == addKeyValue(key, val))
                throw EXCEPTION("Invalid/Duplicate key='" << key << "', value='" << val <<"'");
        }
    }
    
    ////////////////////////////////////////////////////////////////////////////
    template<typename T>
    T getValue(const std::string& key_, const T& default_)
    {
        auto it = find(key_);
        if(it != end())
            return boost::lexical_cast<T>(it->second);
        return default_;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    std::string getValue(const std::string& key_, const char* default_)
    {
        auto it = find(key_);
        if(it != end())
            return it->second;
        return default_;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    template<typename T>
    bool addKeyValue(const std::string& key_, const T& val_)
    {
        return emplace(key_, boost::lexical_cast<std::string>(val_)).second;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    template<typename T>
    std::vector<T> getVector(const std::string& key_)
    {
        std::istringstream iss(getValue(key_,std::string()));
        std::string str;
        std::vector<T> vec;
        while(iss >> str)
        {
            vec.push_back(boost::lexical_cast<T>(str));
        }
        return vec;
    }
};
} // namespace tools
