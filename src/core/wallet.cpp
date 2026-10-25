#include "core/wallet.hpp"
#include <stdexcept>
#include <iostream>

namespace core {

static std::string to_hex(const unsigned char* data, size_t len) {
    static constexpr char hexmap[] = "0123456789abcdef";
    std::string out;
    out.reserve(len * 2);
    for (size_t i = 0; i < len; ++i) {
        out.push_back(hexmap[data[i] >> 4]);
        out.push_back(hexmap[data[i] & 0x0F]);
    } 
    return out;
}

static std::vector<unsigned char> from_hex(const std::string& hex) {
    std::vector<unsigned char> out;
    out.reserve(hex.size() / 2);
    for (size_t i = 0; i < hex.size(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        unsigned char byte = (unsigned char) strtol(byteString.c_str(), nullptr, 16);
        out.push_back(byte);
    }
    return out;
}

Wallet::Wallet() {
    unsigned char pk[crypto_sign_PUBLICKEYBYTES];
    unsigned char sk[crypto_sign_SECRETKEYBYTES];

    crypto_sign_keypair(pk, sk);

    public_key_hex = to_hex(pk, crypto_sign_PUBLICKEYBYTES);
    private_key_hex = to_hex(sk, crypto_sign_SECRETKEYBYTES);
}

Wallet Wallet::from_private_key_hex(const std::string& priv_hex) {
    if (priv_hex.size() != crypto_sign_SECRETKEYBYTES * 2) {
        throw std::runtime_error("invalid private key length");
    }
    Wallet w;
    w.private_key_hex = priv_hex;

    auto sk = from_hex(priv_hex);
    unsigned char pk[crypto_sign_PUBLICKEYBYTES];
    crypto_sign_ed25519_sk_to_pk(pk, sk.data());

    w.public_key_hex = to_hex(pk, crypto_sign_PUBLICKEYBYTES);
    return w;
}

std::string Wallet::sign(const std::string& message_hash) const {
    auto sk = from_hex(private_key_hex);
    auto msg = from_hex(message_hash);

    unsigned char sig[crypto_sign_BYTES];
    unsigned long long sig_len;
    
    crypto_sign_detached(sig, &sig_len, msg.data(), msg.size(), sk.data());

    return to_hex(sig, crypto_sign_BYTES);
}

bool verify_signature(const std::string& public_key_hex,
                      const std::string& message_hash,
                      const std::string& signature_hex) {
    auto pk = from_hex(public_key_hex);
    auto msg = from_hex(message_hash);
    auto sig = from_hex(signature_hex);
    
    if (pk.size() != crypto_sign_PUBLICKEYBYTES) return false;
    if (sig.size() != crypto_sign_BYTES) return false;

    return crypto_sign_verify_detached(sig.data(), msg.data(), msg.size(), pk.data()) == 0;
}

} //end