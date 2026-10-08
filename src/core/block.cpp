#include "core/block.hpp"
#include "crypto/hash.hpp"
#include <sstream>

namespace core {

std::string Block::compute_hash() const{
    std::ostringstream oss;
    oss << index << '|'
        << timestamp << '|'
        << data << '|'
        << prev_hash << '|'
        << nonce;
    return crypto::sha256_hex(oss.str());
}


void Block::mine(uint32_t difficulty){
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
        << "    data=" << data << '\n'
        << "    prev=" << prev_hash.substr(0, 16) << "...\n"
        << "    hash=" << hash.substr(0, 16) << "...";
    return oss.str();
}

}