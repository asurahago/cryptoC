#pragma once
#include <string>
#include <vector>
#include <array>
#include <sodium.h>

namespace core {

class Wallet {
    public:
        std::string public_key_hex;
        std::string private_key_hex;

        Wallet();

        static Wallet from_private_key_hex(const std::string& priv_hex);

        std::string sign(const std::string& message_hash) const;
};

bool verify_signature(const std::string& public_key_hex,
                      const std::string& message_hash,
                      const std::string& signature_hex);

}