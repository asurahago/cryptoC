#include <iostream>
#include <core/chain.hpp>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"

int main() {
    std::cout << CYAN << "=== cryptoC v0.1.0 ===\n" << RESET;
    
    core::Chain chain(3);
    
    std::cout << "Initializing genesis block...\n";
    chain.init_genesis("Genesis Block Data");
    
    std::cout << "Mining block 1...\n";
    chain.add_block("Alice -> Bob: 50 coins");
    
    std::cout << "Mining block 2...\n";
    chain.add_block("Bob -> Charlie: 20 coins");

    std::cout << YELLOW << "\n--- Current Chain ---\n" << RESET;
    for (const auto& block : chain.blocks()) {
        std::cout << block.to_string() << "\n\n";
    }

    std::cout << YELLOW << "--- Validating chain ---\n" << RESET;
    auto result = chain.validate();
    if (result.ok) {
        std::cout << GREEN << "[OK] " << result.reason << RESET << "\n\n";
    } else {
        std::cout << RED << "[ERROR] " << result.reason << RESET << "\n\n";
    }

    std::cout << RED << "--- HACKING THE CHAIN (Block 1) ---\n" << RESET;
  
    auto& mutable_blocks = const_cast<std::vector<core::Block>&>(chain.blocks());
    
    mutable_blocks[1].data = "Alice -> Bob: 999999 coins (HACKED)";
    std::cout << "Data in block 1 has been changed!\n\n";

    std::cout << YELLOW << "--- Validating chain after hack ---\n" << RESET;
    result = chain.validate();
    if (result.ok) {
        std::cout << GREEN << "[OK] " << result.reason << RESET << "\n\n";
    } else {
        std::cout << RED << "[ERROR] " << result.reason << RESET << "\n\n";
    }

    return 0;
}