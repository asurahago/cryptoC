#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "core/transaction.hpp"

namespace core {
    
struct Block {
    uint64_t    index   = 0;
    uint64_t    timestamp   = 0;
    std::vector<Transaction> transactions; //now we storage data in dynamic_list. so, we close to something real...
    std::string prev_hash; // snake_case is my missing...
    uint64_t    nonce   = 0;
    std::string hash;
    std::string merkle_root; //list of transaction now have one hash, good practice.
    
    std::string compute_merkle_root() const;
    std::string compute_hash() const;
    void mine(uint32_t difficulty);
    std::string to_string() const;
};

} // namespace core

