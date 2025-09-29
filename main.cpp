#include "CipherLibrary.hpp"

#include <iostream>
#include <fstream> // операции с файлами
#include <locale>

#include <cstdlib>
#include <ctime>

// using namespace std;

using std::cout;
using std::endl;

std::vector<unsigned char> readBinaryFile(const std::string filename) {
    // Open the file in binary mode and seek to the end to determine its size
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) { throw std::runtime_error("Failed to open file: " + filename); }

    // Get the file size
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read the file into a vector
    std::vector<unsigned char> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw std::runtime_error("Failed to read file: " + filename);
    }

    file.close();

    return buffer;
}

bool writeBinaryFile(const std::string filename, string str){
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(str.c_str(), str.size());
    } else {
        std::cerr << "Не удалось открыть " << filename << std::endl;
        return false;
    }
    file.close();
    return true;
}

int main() {
    srand(time(0));
    setlocale(LC_ALL, "Russian");

    Scrambler scrambler(std::make_unique<Vernam>());

    while (true){
        std::cout << endl << "=========================" << endl;
        
        char choice;
        std::cout << "Выберите действие:" << std::endl;
        std::cout << "1 - зашифровать сообщение;" << std::endl;
        std::cout << "2 - расшифровать код из файла;" << std::endl;
        std::cout << "3 - сперва зашифрует, потом расшифрует;" << std::endl;
        std::cout << "9 - выход" << std::endl;
        std::cout << "Ваш выбор: ";
        std::cin >> choice;
        switch(choice){
            case '1':
            {
                std::cout << endl << "зашифровка сообщения из файла..." << std::endl;
                
                std::string message_from_file;

                std::ifstream input("input.txt");
                if (input.is_open()) {
                    std::getline(input, message_from_file);
                } else {
                    std::cerr << "Не удалось открыть input.txt" << std::endl;
                    return 1;
                }
                input.close();

                pair <string, string> pair = scrambler.encode(message_from_file);

                std::cout << "Исходное сообщение: " << message_from_file << std::endl;
                std::cout << "Сгенерированный ключ: " << pair.second << std::endl;
                std::cout << "Зашифрованное сообщение: " << pair.first << std::endl;

                writeBinaryFile("codes.txt", pair.first);
                writeBinaryFile("keys.txt", pair.second);

                std::cout << "зашифровка закончена" << std::endl;
                break;
            }
            case '2':
            {
                std::cout << endl << "расшифровка кода из файла..." << std::endl;

                std::string codeFromFile, keyFromFile, decoded_mesage;

                std::vector<unsigned char> code_bytes = readBinaryFile("codes.txt");
                for(int i=0; i<code_bytes.size(); i++){
                    codeFromFile += static_cast<char>(code_bytes[i]);
                }

                std::vector<unsigned char> key_bytes = readBinaryFile("keys.txt");
                for(int i=0; i<key_bytes.size(); i++){
                    keyFromFile += static_cast<char>(key_bytes[i]);
                }

                decoded_mesage = scrambler.decode(codeFromFile, keyFromFile);

                std::cout << "Зашифрованное сообщение: " << codeFromFile << std::endl;
                std::cout << "Ключ: " << keyFromFile << std::endl;
                std::cout << "Расшифрованное сообщение: " << decoded_mesage << std::endl;
                
                std::cout << "расшифровка закончена" << std::endl;
                break;
            }
            case '3':
            {
                std::cout << endl << "шифрование и дешифрование..." << std::endl;

                string message_from_file, code_from_file, key_from_file, decoded_mesage;
                
                std::ifstream input("input.txt");
                if (input.is_open()) {
                    std::getline(input, message_from_file);
                } else {
                    std::cerr << "Не удалось открыть input.txt" << std::endl;
                    return 1;
                }
                input.close();

                pair <string, string> pair = scrambler.encode(message_from_file);

                writeBinaryFile("codes.txt", pair.first);
                writeBinaryFile("keys.txt", pair.second);

                std::cout << "зашифровка закончена" << std::endl;


                std::vector<unsigned char> code_bytes = readBinaryFile("codes.txt");
                for(int i=0; i<code_bytes.size(); i++){
                    code_from_file += static_cast<char>(code_bytes[i]);
                }

                std::vector<unsigned char> key_bytes = readBinaryFile("keys.txt");
                for(int i=0; i<key_bytes.size(); i++){
                    key_from_file += static_cast<char>(key_bytes[i]);
                }

                decoded_mesage = scrambler.decode(code_from_file, key_from_file);

                std::cout << "расшифровка закончена" << std::endl;

                std::cout << "Исходное сообщение: " << message_from_file << std::endl;
                std::cout << "Сгенерированный ключ: " << key_from_file << std::endl;
                std::cout << "Зашифрованное сообщение: " << code_from_file << std::endl;
                std::cout << "Расшифрованное сообщение: " << decoded_mesage << std::endl;

                if(message_from_file == decoded_mesage){
                    cout << "EQUAL!!!!!!" << endl;
                } else{
                    cout << "DIFFERENT!" << endl;
                }
                break;
            }
            case '9':
            {
                exit(0);
            }
        }
    }
    return 0;
}