#pragma once
#include "core/block.hpp"
#include "core/mempool.hpp"
#include <vector>
#include <string>

namespace core{

struct ValidationResult{
    bool ok = true;
    std::string reason;
};

class Chain {
    public:

        static constexpr uint64_t BLOCK_REWARD = 42;
        static constexpr uint64_t MAX_SUPPLY = 24'000'000;
        static constexpr const char* COINBASE_SENDER = "COINBASE";


        explicit Chain(uint32_t difficulty);
        void init_genesis(const std::string& genesis_data = "genesis");

        Block& mine_block(Mempool& pool, const std::string& miner_address, size_t max_txs = 10);
        ValidationResult validate() const;

        uint64_t get_balance(const std::string& address) const;
        uint64_t total_supply() const;

        const std::vector<Block>& blocks() const;
        uint32_t difficulty() const;

    private:
        std::vector<Block> blocks_;
        uint32_t difficulty_;
        static std::string zeros64();
};

}