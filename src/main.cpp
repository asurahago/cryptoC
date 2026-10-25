#include <iostream>
#include <core/wallet.hpp>
#include <core/transaction.hpp> // Мы создадим его чуть позже, но пока закомментируй

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define CYAN    "\033[36m"

int main() {
    std::cout << CYAN << "=== Testing Cryptography ===\n" << RESET;
    
    // 1. Создаем кошельки Алисы и Боба
    core::Wallet alice;
    core::Wallet bob;

    std::cout << "Alice Address: " << alice.public_key_hex.substr(0, 16) << "...\n";
    std::cout << "Bob Address:   " << bob.public_key_hex.substr(0, 16) << "...\n\n";

    // 2. Алиса хочет отправить 10 монет. Формируем данные и хеш.
    std::string message = "Alice sends 10 coins to Bob";
    std::string message_hash = crypto::sha256_hex(message); 
    
    std::cout << "Message: " << message << "\n";
    std::cout << "Hash:    " << message_hash.substr(0, 16) << "...\n\n";

    // 3. Алиса подписывает хеш своим приватным ключом
    std::string signature = alice.sign(message_hash);
    std::cout << "Signature: " << signature.substr(0, 16) << "...\n\n";

    // 4. Сеть (мы) проверяет подпись публичным ключом Алисы
    bool is_valid = core::verify_signature(alice.public_key_hex, message_hash, signature);
    
    if (is_valid) {
        std::cout << GREEN << "[OK] Signature is VALID. Alice authorized this.\n" << RESET;
    } else {
        std::cout << RED << "[ERROR] Signature is INVALID.\n" << RESET;
    }

    // 5. СИМУЛЯЦИЯ ВЗЛОМА: Хакер меняет сообщение!
    std::cout << "\n--- Hacker changes the message ---\n";
    std::string hacked_message = "Alice sends 1000 coins to Bob";
    std::string hacked_hash = crypto::sha256_hex(hacked_message);
    
    std::cout << "Hacked Hash: " << hacked_hash.substr(0, 16) << "...\n";
    
    // Пытаемся проверить СТАРУЮ подпись с НОВЫМ хешем
    bool is_hacked_valid = core::verify_signature(alice.public_key_hex, hacked_hash, signature);
    
    if (is_hacked_valid) {
        std::cout << GREEN << "[OK] Signature is VALID (This should NOT happen!)\n" << RESET;
    } else {
        std::cout << RED << "[ERROR] Signature is INVALID. The hack failed!\n" << RESET;
    }

    return 0;
}