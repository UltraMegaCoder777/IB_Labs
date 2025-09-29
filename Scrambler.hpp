#pragma once
#include <memory> //for unique_ptr
#include "ScramblerMethod.hpp"

class Scrambler
{
private:
    std::unique_ptr<ScramblerMethod> method;
public:
    explicit Scrambler(std::unique_ptr<ScramblerMethod> customMethod)
        : method(std::move(customMethod)) {}
    
    void setMethod(std::unique_ptr<ScramblerMethod> newMethod) {
        method = std::move(newMethod);
    }

    std::string encode(const std::string& message, const std::string& key) {
        return method->encode(message, key);
    }
    pair <string, string> encode(const std::string& message) {
        return method->encode(message);
    }

    std::string decode(const std::string& code, const std::string& key) {
        return method->decode(code, key);
    }
};