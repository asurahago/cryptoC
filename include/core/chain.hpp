#pragma once
#include "core/block.hpp"
#include <vector>
#include <string>

namespace core{

struct ValidationResult{
    bool ok = true;
    std::string reason;
};

class Chain {
    public:
        explicit Chain(uint32_t difficulty);
        void init_genesis(const std::string& genesis_data = "genesis");
        Block& add_block(const std::string& data);
        ValidationResult validate() const;

        const std::vector<Block>& blocks() const;
        uint32_t difficulty() const;

    private:
        std::vector<Block> blocks_;
        uint32_t difficulty_;
        static std::string zeros64();
};

}