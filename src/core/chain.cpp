#include "core/chain.hpp"
#include "crypto/hash.hpp"
#include <chrono>

namespace core {


static uint64_t now_sec() {
    return  std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

std::string Chain::zeros64(){
    return std::string(64, '0');
}

Chain::Chain(uint32_t difficulty) : difficulty_(difficulty) {}

void Chain::init_genesis(const std::string& genesis_data) {
    (void)genesis_data;
    blocks_.clear();
    Block g;
    g.index = 0;
    g.timestamp = now_sec();
    g.prev_hash = zeros64();
    g.mine(difficulty_);
    blocks_.push_back(std::move(g));
}

Block& Chain::mine_block(Mempool& pool, const std::string& miner_address, size_t max_txs) {
    const Block& last = blocks_.back();

    Block b;
    b.index     = last.index + 1;
    b.timestamp = now_sec();
    b.prev_hash = last.hash;

    //coinbase
    if(total_supply() + BLOCK_REWARD <= MAX_SUPPLY) {
        Transaction cb;
        cb.sender   = COINBASE_SENDER;
        cb.receiver = miner_address;
        cb.amount   = BLOCK_REWARD;
        cb.nonce    = 0;
        b.transactions.push_back(std::move(cb));
    }

    auto txs = pool.get_transactions(max_txs);
    for (auto& t : txs) b.transactions.push_back(std::move(t));

    b.mine(difficulty_);
    blocks_.push_back(std::move(b));
    return blocks_.back();
}

ValidationResult Chain::validate() const {
    for (size_t i = 1; i < blocks_.size(); ++i) {
        const Block& cur = blocks_[i];
        const Block& prev = blocks_[i - 1];

        if(cur.prev_hash != prev.hash) {
            return {false, "invalid hash: data tampered at block " + std::to_string(i)};
        }

        if(cur.compute_merkle_root() != cur.merkle_root) {
            return {false, "merkle root mismatch at block: data tampared " + std::to_string(i)};
        }

        if(cur.compute_hash() != cur.hash) {
            return {false, "Invalid hash: data tampared at block " + std::to_string(i)};
        }

        if(!crypto::has_leading_zeros(cur.hash, static_cast<int>(difficulty_))) {
            return {false, "invalid difficulty: hash does not meet target at block " + std::to_string(i)};
        }
    }
    return {true, "Chain is valid"};
}

uint64_t Chain::get_balance(const std::string& address) const {
    int64_t balance = 0;
    for (const auto& b : blocks_) {
        for(const auto& tx : b.transactions) {
            if (tx.sender == address) balance -= static_cast<int64_t>(tx.amount);
            if (tx.receiver == address) balance += static_cast<int64_t>(tx.amount);
        }
    }
    return balance < 0 ? 0 : static_cast<uint64_t>(balance);
}

uint64_t Chain::total_supply() const {
    uint64_t s = 0;
    for (const auto& b : blocks_) {
        for (const auto& tx : b.transactions) {
            if (tx.sender == COINBASE_SENDER) s += tx.amount;
        }
    }
    return s;
}

const std::vector<Block>& Chain::blocks() const {
    return blocks_;
}

uint32_t Chain::difficulty() const {
    return difficulty_;
}

}