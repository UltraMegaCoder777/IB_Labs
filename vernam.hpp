#pragma once
#include "ScramblerMethod.hpp"

class Vernam : public ScramblerMethod
{
private:
    string generateKey(int messageLength) {
        string key;
        for (int i = 0; i < messageLength; i++) {
            // Генерируем случайный байт (включая кириллицу)
            key.push_back(rand() % 256);
        }
        return key;
    }

public:
    string encode(const string& message, const string& key) override {
        vector <uint8_t> message_bytes = getBytes(message);
        vector <uint8_t> key_bytes = getBytes(key);
        if (key_bytes.size() < message_bytes.size()) {
            throw runtime_error("Key is too short for the message");
        }
        string code;
        for (int i = 0; i < message_bytes.size(); i++) {
            code.push_back(message_bytes[i] ^ key_bytes[i]);
        }
        return code;
    }
    pair <string, string> encode(const string& message) override {
        vector <uint8_t> messageBytes = getBytes(message);
        string key = generateKey(messageBytes.size());
        string code = encode(message, key);
        pair result = make_pair(code, key);
        return result;
    }

    string decode(const string& code, const string& key) override {
        return encode(code, key);
    }
};