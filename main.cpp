#include "CipherLibrary.hpp"

#include <iostream>
#include <fstream> // операции с файлами
#include <locale>

#include <cstdlib>
#include <ctime>

using std::cout;
using std::cin;
using std::endl;

vector <uint8_t> readBinaryFile(const string filename) {
    // Open the file in binary mode and seek to the end to determine its size
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) { throw std::runtime_error("Failed to open file: " + filename); }

    // Get the file size
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read the file into a vector
    vector <uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw std::runtime_error("Failed to read file: " + filename);
    }

    file.close();

    return buffer;
}

bool writeBinaryFile(const string filename, string str){
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(str.c_str(), str.size());
    } else {
        std::cerr << "Не удалось открыть " << filename << endl;
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
        cout << endl << "=========================" << endl;
        
        char choice;
        cout << "Выберите действие:" << endl;
        cout << "1 - зашифровать сообщение;" << endl;
        cout << "2 - расшифровать код из файла;" << endl;
        cout << "3 - сперва зашифрует, потом расшифрует;" << endl;
        cout << "9 - выход" << endl;
        cout << "Ваш выбор: ";
        cin >> choice;
        switch(choice){
            case '1':
            {
                cout << endl << "зашифровка сообщения из файла..." << endl;
                
                string message_from_file;

                std::ifstream input("input.txt");
                if (input.is_open()) {
                    std::getline(input, message_from_file);
                } else {
                    std::cerr << "Не удалось открыть input.txt" << endl;
                    return 1;
                }
                input.close();

                pair <string, string> pair = scrambler.encode(message_from_file);

                cout << "Исходное сообщение: " << message_from_file << endl;
                cout << "Сгенерированный ключ: " << pair.second << endl;
                cout << "Зашифрованное сообщение: " << pair.first << endl;

                writeBinaryFile("codes.txt", pair.first);
                writeBinaryFile("keys.txt", pair.second);

                cout << "зашифровка закончена" << endl;
                break;
            }
            case '2':
            {
                cout << endl << "расшифровка кода из файла..." << endl;

                string codeFromFile, keyFromFile, decoded_mesage;

                vector <uint8_t> code_bytes = readBinaryFile("codes.txt");
                for(int i=0; i<code_bytes.size(); i++){
                    codeFromFile += static_cast<char>(code_bytes[i]);
                }

                vector <uint8_t> key_bytes = readBinaryFile("keys.txt");
                for(int i=0; i<key_bytes.size(); i++){
                    keyFromFile += static_cast<char>(key_bytes[i]);
                }

                decoded_mesage = scrambler.decode(codeFromFile, keyFromFile);

                cout << "Зашифрованное сообщение: " << codeFromFile << endl;
                cout << "Ключ: " << keyFromFile << endl;
                cout << "Расшифрованное сообщение: " << decoded_mesage << endl;
                
                cout << "расшифровка закончена" << endl;
                break;
            }
            case '3':
            {
                cout << endl << "шифрование и дешифрование..." << endl;

                string message_from_file, code_from_file, key_from_file, decoded_mesage;
                
                std::ifstream input("input.txt");
                if (input.is_open()) {
                    std::getline(input, message_from_file);
                } else {
                    std::cerr << "Не удалось открыть input.txt" << endl;
                    return 1;
                }
                input.close();

                pair <string, string> pair = scrambler.encode(message_from_file);

                writeBinaryFile("codes.txt", pair.first);
                writeBinaryFile("keys.txt", pair.second);

                cout << "зашифровка закончена" << endl;


                vector <uint8_t> code_bytes = readBinaryFile("codes.txt");
                for(int i=0; i<code_bytes.size(); i++){
                    code_from_file += static_cast<char>(code_bytes[i]);
                }

                vector <uint8_t> key_bytes = readBinaryFile("keys.txt");
                for(int i=0; i<key_bytes.size(); i++){
                    key_from_file += static_cast<char>(key_bytes[i]);
                }

                decoded_mesage = scrambler.decode(code_from_file, key_from_file);

                cout << "расшифровка закончена" << endl;

                cout << "Исходное сообщение: " << message_from_file << endl;
                cout << "Сгенерированный ключ: " << key_from_file << endl;
                cout << "Зашифрованное сообщение: " << code_from_file << endl;
                cout << "Расшифрованное сообщение: " << decoded_mesage << endl;

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