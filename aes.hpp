#ifndef AES_HPP
#define AES_HPP

#include <string>

// Simple XOR-based AES-like encryption class
class AES {
    std::string key; // Encryption key (used for XOR)

public:
    // Constructor: sets the encryption key
    AES(const std::string& k) : key(k) {}

    // Encrypts the input text using XOR with the key
    std::string encrypt(const std::string& text) {
        std::string res = text;
        for (size_t i = 0; i < res.size(); ++i)
            res[i] ^= key[i % key.size()]; // XOR each character with the repeating key
        return res;
    }

    // Decrypts the input text (same as encryption, since XOR is symmetric)
    std::string decrypt(const std::string& text) {
        return encrypt(text);  // Reapplying XOR decrypts the message
    }
};

#endif // AES_HPP
