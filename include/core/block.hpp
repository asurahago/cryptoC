#pragma once
#include <cstdint>
#include <string>

namespace core {
    
struct Block {
    uint64_t    index   = 0;
    uint64_t    timestamp   = 0;
    std::string data;
    std::string prev_hash; // snake_case is my missing...
    uint64_t    nonce   = 0;
    std::string hash;
    
    std::string compute_hash() const;
    void mine(uint32_t difficulty);
    std::string to_string() const;
};

} // namespace core

