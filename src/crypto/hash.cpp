#include "crypto/hash.hpp"
#include <sodium.h>
#include <array>
#include <stdexcept>

namespace crypto {
    std::string sha256_hex(const std::string& data){
        static const bool initialized = [] {
            if(sodium_init() < 0) throw std::runtime_error("sodium_init failed");
            return true;
        }();
        (void)initialized;

        std::array<unsigned char, crypto_hash_sha256_BYTES> digest{};

        crypto_hash_sha256(
            digest.data(),
            reinterpret_cast<const unsigned char*>(data.data()),
            data.size()
        );

        static constexpr char hexmap[] = "0123456789abcdef";
        std::string out;
        out.reserve(digest.size() * 2);
        for (unsigned char b : digest) {
            out.push_back(hexmap[b >> 4]);
            out.push_back(hexmap[b & 0x0F]);
        }
        return out;

    }

    bool has_leading_zeros(const std::string& hex, int difficulty) {
        if (difficulty < 0) return true;
        if (static_cast<size_t>(difficulty) > hex.size()) return false;
        for (int i = 0; i < difficulty; ++i) {
            if(hex[i] != '0') return false;
        }
        return true;
    }
}