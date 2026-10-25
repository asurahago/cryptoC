#pragma once
#include <vector>
#include <mutex>
#include "core/transaction.hpp"

namespace core {

class Mempool {
    private:
        std::vector<Transaction> transactions_;
        mutable std::mutex mutex_;

    public:
        Mempool() = default;

        bool add_transaction(const Transaction& tx);
        std::vector<Transaction> get_transactions(size_t max_count = 100);
        size_t size() const;
        void clear();
};

}