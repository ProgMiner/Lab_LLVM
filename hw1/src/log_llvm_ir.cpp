#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <csignal>
#include <vector>
#include <atomic>
#include <deque>


namespace std {

    template<typename T>
    struct hash<std::vector<T>> {

        std::size_t operator()(std::vector<T> const & vec) const {
            std::size_t seed = vec.size();
            std::hash<T> h;

            for (auto & i : vec) {
                seed ^= h(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }

            return seed;
        }
    };
}

static std::unordered_map<std::vector<const char *>, uint64_t> stats;
static std::deque<const char *> windows[6];

static std::atomic_bool sig = false;

extern "C" void log_llvm_ir(const char * instruction) {
    for (uint64_t i = 1; i <= 5; ++i) {
        windows[i].push_back(instruction);

        if (windows[i].size() > i) {
            windows[i].pop_front();
        }

        if (windows[i].size() == i) {
            ++stats[std::vector(windows[i].begin(), windows[i].end())];
        }
    }

    if (sig) {
        std::vector<std::pair<std::vector<const char *>, uint64_t>> sorted_stats;

        sorted_stats.reserve(stats.size());
        std::move(stats.begin(), stats.end(), std::back_inserter(sorted_stats));

        std::sort(sorted_stats.begin(), sorted_stats.end(), [](const auto & a, const auto & b) {
            return a.second < b.second;
        });

        for (auto & [k, v] : sorted_stats) {
            std::cout << "===== " << v << " =====" << std::endl;

            for (auto & s : k) {
                std::cout << s << std::endl;
            }
        }

        std::terminate();
    }
}

static void handle_signal(int) {
    sig = true;
}

__attribute__((constructor))
static void init_logger() {
    struct sigaction sa {};

    sa.sa_handler = handle_signal;

    sigaction(SIGTERM, &sa, nullptr);
    sigaction(SIGINT, &sa, nullptr);
}
