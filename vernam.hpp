#pragma once
#include "ScramblerMethod.hpp"

#include <cstdlib>
#include <ctime>

class Vernam : public ScramblerMethod
{
private:
    std::string generateKey(int messageLength) {
        std::string key;
        for (int i = 0; i < messageLength; i++) {
            // Генерируем случайный байт (включая кириллицу)
            key.push_back(rand() % 256);
        }
        return key;
    }

public:
    std::string encode(const std::string& message, const std::string& key) override {
        std::vector <unsigned char> message_bytes = getBytes(message);
        std::vector <unsigned char> key_bytes = getBytes(key);
        if (key_bytes.size() < message_bytes.size()) {
            throw std::runtime_error("Key is too short for the message");
        }
        std::string code;
        for (int i = 0; i < message_bytes.size(); i++) {
            code.push_back(message_bytes[i] ^ key_bytes[i]);
        }
        return code;
    }
    pair <string, string> encode(const std::string& message) override {
        std::vector <unsigned char> messageBytes = getBytes(message);
        std::string key = generateKey(messageBytes.size());
        std::string code = encode(message, key);
        pair result = make_pair(code, key);
        return result;
    }

    std::string decode(const std::string& code, const std::string& key) override {
        return encode(code, key);
    }
};