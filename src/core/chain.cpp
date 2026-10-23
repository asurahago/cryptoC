#include "core/chain.hpp"
#include "crypto/hash.hpp"
#include <chrono>
#include <iostream>

namespace core {

std::string Chain::zeros64(){
    return std::string(64, '0');
} //genesis block, first block in chain

Chain::Chain(uint32_t difficulty) : difficulty_(difficulty) {
    //empty
}

void Chain::init_genesis(const std::string& genesis_data) {
    blocks_.clear();

    Block genesis;
    genesis.index = 0;
    genesis.timestamp = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    genesis.data = genesis_data;
    genesis.prev_hash = zeros64();

    genesis.mine(difficulty_);
    blocks_.push_back(genesis);
}

Block& Chain::add_block(const std::string& data) {
    const Block& last_block = blocks_.back();

    Block new_block;
    new_block.index = last_block.index + 1;
    new_block.timestamp = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
    new_block.data = data;
    new_block.prev_hash = last_block.hash;

    new_block.mine(difficulty_);
    blocks_.push_back(new_block);

    return blocks_.back();
}

ValidationResult Chain::validate() const {
    for (size_t i = 1; i < blocks_.size(); ++i) {
        const Block& current = blocks_[i];
        const Block& previous = blocks_[i - 1];

        if(current.prev_hash != previous.hash) {
            return {false, "invalid hash: data tampered at block " + std::to_string(i)};
        }

        if(current.compute_hash() != current.hash) {
            return {false, "Invalid hash: data tampared at block " + std::to_string(i)};
        }

        if(!crypto::has_leading_zeros(current.hash, static_cast<int>(difficulty_))) {
            return {false, "invalid difficulty: hash does not meet target at block " + std::to_string(i)};
        }
    }
    return {true, "Chain is valid"};
}

const std::vector<Block>& Chain::blocks() const {
    return blocks_;
}

uint32_t Chain::difficulty() const {
    return difficulty_;
}

}