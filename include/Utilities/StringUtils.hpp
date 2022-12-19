#pragma once

#include <string>
#include <vector>
#include <regex>

namespace claid
{
    static void stringReplaceAll(std::string& str, const std::string from, const std::string to)
    {
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos) 
        {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
            printf("%s\n", str.c_str());
        }
    }

    static void splitStringToVector(const std::string& str, const std::string delimiter, std::vector<std::string>& output)
    {
        output.clear();
        size_t last = 0; 
        size_t next = 0; 
        while ((next = str.find(delimiter, last)) != std::string::npos) 
        {   
            output.push_back(str.substr(last, next-last));   
            last = next + 1;
        } 
        output.push_back(str.substr(last));
    }
}