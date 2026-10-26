#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "core/chain.hpp"
#include "core/mempool.hpp"
#include "core/wallet.hpp"

using namespace ftxui;

int main() {
    // ─── ядро блокчейна ───
    core::Chain   chain(3);
    core::Mempool pool;
    chain.init_genesis();

    core::Wallet alice, bob, miner;

    // ─── состояние для UI (защищено мьютексом) ───
    std::mutex        state_mutex;
    std::atomic<bool> mining{false};
    std::string       last_event = "ready";
    uint64_t          alice_nonce = 0;

    // ─── поток майнера (храним, чтобы join'ить) ───
    std::thread miner_thread;

    auto screen = ScreenInteractive::Fullscreen();

    // ─── действие: создать тестовую транзакцию ───
    auto make_tx = [&] {
        std::lock_guard lock(state_mutex);
        core::Transaction tx;
        tx.sender    = alice.public_key_hex;
        tx.receiver  = bob.public_key_hex;
        tx.amount    = 5;
        tx.nonce     = ++alice_nonce;
        tx.signature = alice.sign(tx.get_hash());
        if (core::verify_signature(tx.sender, tx.get_hash(), tx.signature)) {
            pool.add_transaction(tx);
            last_event = "tx queued";
        } else {
            last_event = "tx REJECTED";
        }
    };

    // ─── действие: майнить один блок в фоне ───
    auto trigger_mine = [&] {
        if (mining.load()) return;                       // уже майнит
        if (miner_thread.joinable()) miner_thread.join(); // дождаться прошлого

        mining = true;
        last_event = "mining...";
        miner_thread = std::thread([&] {
            {
                std::lock_guard lock(state_mutex);
                chain.mine_block(pool, miner.public_key_hex);
                last_event = "block mined";
            }
            mining = false;
            screen.PostEvent(Event::Custom);             // попросить UI перерисоваться
        });
    };

    // ─── рендер одного блока ───
    auto render_block = [](const core::Block& b) {
        return vbox({
            hbox({
                text("#" + std::to_string(b.index)) | bold | color(Color::Cyan),
                text("  " + b.hash.substr(0, 12) + "...") | dim,
            }),
            hbox({
                text("  txs=" + std::to_string(b.transactions.size())),
                text("  nonce=" + std::to_string(b.nonce)) | dim,
            }),
        });
    };

    // ─── рендер транзакции ───
    auto render_tx = [](size_t i, const core::Transaction& tx) {
        return hbox({
            text("tx" + std::to_string(i) + " ") | color(Color::Yellow),
            text(tx.sender.substr(0, 6) + "→" + tx.receiver.substr(0, 6)),
            text(" " + std::to_string(tx.amount)) | color(Color::Green),
        });
    };

    // ─── компоновка UI ───
    auto component = Renderer([&] {
        // снимок состояния под мьютексом
        std::vector<core::Block>       blocks_copy;
        std::vector<core::Transaction> txs_copy;
        size_t   pool_size;
        uint64_t supply;
        std::string ev;
        bool     is_mining;
        {
            std::lock_guard lock(state_mutex);
            blocks_copy = chain.blocks();
            txs_copy    = pool.peek();          // ← readonly peek, без удаления
            supply      = chain.total_supply();
            ev          = last_event;
            is_mining   = mining.load();
        }
        pool_size = txs_copy.size();

        // ─── панель chain ───
        Elements block_elements;
        for (const auto& b : blocks_copy) {
            block_elements.push_back(render_block(b));
            block_elements.push_back(separatorLight());
        }
        auto chain_panel = window(
            text(" chain · " + std::to_string(blocks_copy.size()) + " blocks "),
            vbox(std::move(block_elements))
        );

        // ─── панель mempool ───
        Elements tx_elements;
        if (txs_copy.empty()) {
            tx_elements.push_back(text("(empty)") | dim);
        } else {
            for (size_t i = 0; i < txs_copy.size(); ++i) {
                tx_elements.push_back(render_tx(i, txs_copy[i]));
            }
        }
        auto mempool_panel = window(
            text(" mempool · " + std::to_string(pool_size) + " tx "),
            vbox(std::move(tx_elements))
        );

        // ─── панель статуса ───
        auto status_panel = window(text(" status "), vbox({
            hbox({text("mining:     "), is_mining ? text("YES") | color(Color::Red) | bold
                                                  : text("idle") | dim}),
            hbox({text("supply:     "), text(std::to_string(supply) + " / 24000000")}),
            hbox({text("difficulty: "), text(std::to_string(chain.difficulty()))}),
            separator(),
            hbox({text("last:       "), text(ev) | color(Color::Yellow)}),
        }));

        auto right = vbox({ mempool_panel | flex, status_panel });
        auto body  = hbox({
            chain_panel | flex,
            right | size(WIDTH, EQUAL, 44),
        });

        auto footer = hbox({
            text("[M] "), text("mine") | bold,
            text("   [T] "), text("new tx") | bold,
            text("   [Q] "), text("quit") | bold,
        }) | center | dim;

        return vbox({ body | flex, footer }) | border | bgcolor(Color::Black);
    });

    // ─── обработка клавиш ───
    component |= CatchEvent([&](Event e) {
        if (e == Event::Character('m') || e == Event::Character('M')) { trigger_mine(); return true; }
        if (e == Event::Character('t') || e == Event::Character('T')) { make_tx();     return true; }
        if (e == Event::Character('q') || e == Event::Character('Q') || e == Event::Escape) {
            if (miner_thread.joinable()) miner_thread.join();
            screen.Exit();
            return true;
        }
        return false;
    });

    screen.Loop(component);

    if (miner_thread.joinable()) miner_thread.join();
    return 0;
}