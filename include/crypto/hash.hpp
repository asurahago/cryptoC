#pragma once
#include <string>
#include <cstdint>

namespace crypto {
    std::string sha256_hex(const std::string& data);
    bool has_leading_zeros(const std::string& hex, int difficulty);
}