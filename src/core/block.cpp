#include "core/block.hpp"
#include "crypto/hash.hpp"
#include <sstream>

namespace core {


std::string Block::compute_merkle_root() const {
    if (transactions.empty()) return crypto::sha256_hex("empty");
    std::string concat;
    concat.reserve(transactions.size() * 64);
    for (const auto& tx : transactions) concat += tx.get_hash();
    return crypto::sha256_hex(concat);
}

std::string Block::compute_hash() const{
    std::ostringstream oss;
    oss << index << '|'
        << timestamp << '|'
        << merkle_root << '|'
        << prev_hash << '|'
        << nonce;
    return crypto::sha256_hex(oss.str());
}


void Block::mine(uint32_t difficulty){
    merkle_root = compute_merkle_root();
    nonce = 0;
    while(true){
        hash = compute_hash();
        if(crypto::has_leading_zeros(hash, static_cast<int>(difficulty))){
            return;
        }
        ++nonce;
    }
}


std::string Block::to_string() const{
    std::ostringstream oss;
    oss << "Block #" << index
        << "    ts=" << timestamp
        << "    nonce=" << nonce << '\n'
        << "    merkle=" << merkle_root.substr(0, 16) << '\n'
        << "    prev=" << prev_hash.substr(0, 16) << "...\n"
        << "    hash=" << hash.substr(0, 16) << "..."
        << "    txs=" << transactions.size();
    for (const auto& tx : transactions) oss << "\n  " << tx.to_string();
    return oss.str();
}

}