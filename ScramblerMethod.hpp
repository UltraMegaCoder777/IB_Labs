#pragma once
#include <iostream>

#include <string>
#include <vector>
#include <utility> //for pair
// #include <stdexcept> // отлов ошибок

using std::string;
using std::vector;
using std::pair;
using std::cerr;

class ScramblerMethod
{
protected:
    std::vector<unsigned char> getBytes(const std::string& str) {
        return std::vector<unsigned char>(str.begin(), str.end());
    }
    
public:
    virtual std::string encode(const std::string& message, const std::string& key) = 0;
    virtual std::pair <string, string> encode(const std::string& message) = 0;
    virtual std::string decode(const std::string& code, const std::string& key) = 0;
    virtual ~ScramblerMethod() = default;
};
