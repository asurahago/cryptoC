#include "core/mempool.hpp"

namespace core {

bool Mempool::add_transaction(const Transaction& tx) {
    std::lock_guard<std::mutex> lock(mutex_);
    transactions_.push_back(tx);
    return true;
}

std::vector<Transaction> Mempool::get_transactions(size_t max_count) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<Transaction> result;
    size_t count = std::min(max_count, transactions_.size());
    result.assign(transactions_.begin(), transactions_.begin() + count); //asign and begin, method erase
    
    transactions_.erase(transactions_.begin(), transactions_.begin() + count);
    return result;
}

size_t Mempool::size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return transactions_.size();
}

void Mempool::clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    transactions_.clear();
}

}