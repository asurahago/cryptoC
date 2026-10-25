#include <iostream>
#include <core/wallet.hpp>
#include <core/transaction.hpp>
#include <core/mempool.hpp>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define CYAN    "\033[36m"
#define YELLOW  "\033[33m"

int main() {
    std::cout << CYAN << "=== cryptoC Wallet & Mempool Test ===\n" << RESET;
    
    core::Wallet alice;
    core::Wallet bob;
    
    std::cout << "Alice Addr: " << alice.public_key_hex.substr(0, 16) << "...\n";
    std::cout << "Bob Addr:   " << bob.public_key_hex.substr(0, 16) << "...\n\n";

    core::Transaction tx;
    tx.sender = alice.public_key_hex;
    tx.receiver = bob.public_key_hex;
    tx.amount = 10;
    tx.nonce = 1;

    std::string tx_hash = tx.get_hash();
    tx.signature = alice.sign(tx_hash);

    std::cout << "Created: " << tx.to_string() << "\n\n";

    bool is_valid = core::verify_signature(tx.sender, tx_hash, tx.signature);
    if (is_valid) {
        std::cout << GREEN << "[OK] Signature is valid. Transaction accepted.\n" << RESET;
    } else {
        std::cout << RED << "[ERROR] Signature invalid. Transaction rejected.\n" << RESET;
        return 1;
    }

    core::Mempool mempool;
    mempool.add_transaction(tx);
    std::cout << YELLOW << "\nMempool size: " << mempool.size() << " transaction(s)\n" << RESET;
    std::cout << RED << "\n--- Hacker tampers with the transaction ---\n" << RESET;
    tx.amount = 999999;
    std::string hacked_hash = tx.get_hash();
    
    bool is_hacked_valid = core::verify_signature(tx.sender, hacked_hash, tx.signature);
    if (is_hacked_valid) {
        std::cout << GREEN << "[OK] Hack passed (This should NOT happen!)\n" << RESET;
    } else {
        std::cout << RED << "[ERROR] Hack failed. Signature does not match new data.\n" << RESET;
    }

    return 0;
}