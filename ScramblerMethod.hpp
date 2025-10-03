#pragma once
#include <iostream>

#include <string>
#include <vector>
#include <utility> //for pair
#include <cstdint> // for uint8_t
// #include <stdexcept> // отлов ошибок

using std::string;
using std::vector;
using std::pair;
using std::cerr;
using std::runtime_error;

class ScramblerMethod
{
protected:
    vector <uint8_t> getBytes(const string& str) {
        return vector <uint8_t> (str.begin(), str.end());
    }
    
public:
    virtual string encode(const string& message, const string& key) = 0;
    virtual pair <string, string> encode(const string& message) = 0;
    virtual string decode(const string& code, const string& key) = 0;
    virtual ~ScramblerMethod() = default;
};
