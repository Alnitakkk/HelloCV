#ifndef CRYPTO_H
#define CRYPTO_H
#include <string>

class Crypto {
public:
    static std::string encrypt(const std::string& text, int key);
    static std::string decrypt(const std::string& cipherText, int key);
};
#endif