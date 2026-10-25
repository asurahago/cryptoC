#pragma once
#include <string>
#include <cstdint>
#include "crypto/hash.hpp"

namespace core {

struct Transaction {
    std::string sender; // publickey
    std::string receiver; // secretkey
    uint64_t amount = 0; 
    uint64_t nonce = 0;
    std::string signature; //author_id

    //we give uniq_hash - id transaction
    std::string get_hash() const {
        std::string data = sender + receiver + std::to_string(amount) + std::to_string(nonce);
        return crypto::sha256_hex(data);
    }

    std::string to_string() const {
        return "Tx[" + get_hash().substr(0, 8) + "] " +
               sender.substr(0, 6) + "... -> " +
               receiver.substr(0, 6) + "... | " +
               std::to_string(amount) + " coins | nonce: " + std::to_string(nonce);
    }
};

}