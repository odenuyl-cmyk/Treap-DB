
#include "treap.h"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

using Clock = std::chrono::steady_clock;

double elapsed(Clock::time_point start, Clock::time_point end) {
    return std::chrono::duration<double, std::milli>(end - start).count();
}

struct Results {
    double insertRandom = 0;
    double insertSorted = 0;
    double find = 0;
    double rank = 0;
    double remove = 0;
    double size = 0;
};

Results benchmark(std::size_t n, int repetitions, std::mt19937_64& rng) {
    Results results;

    for (int r = 0; r < repetitions; ++r) {
        std::vector<int> keys(n);
        std::iota(keys.begin(), keys.end(), 0);

        std::vector<int> randomKeys = keys;
        std::shuffle(randomKeys.begin(), randomKeys.end(), rng);

        std::vector<std::size_t> rankQueries(n);
        std::iota(rankQueries.begin(), rankQueries.end(), 0);
        std::shuffle(rankQueries.begin(), rankQueries.end(), rng);

        // Random insertion
        {
            Treap<int, int> treap;

            auto start = Clock::now();
            for (int key : randomKeys) {
                treap.insert(key, key);
            }
            auto end = Clock::now();

            results.insertRandom += elapsed(start, end);
        }

        // Sorted insertion
        {
            Treap<int, int> treap;

            auto start = Clock::now();
            for (int key : keys) {
                treap.insert(key, key);
            }
            auto end = Clock::now();

            results.insertSorted += elapsed(start, end);
        }

        // Find existing keys
        {
            Treap<int, int> treap;
            for (int key : randomKeys) {
                treap.insert(key, key);
            }

            volatile std::int64_t checksum = 0;

            auto start = Clock::now();
            for (int key : randomKeys) {
                int* value = treap.find(key);
                if (value != nullptr) {
                    checksum += *value;
                }
            }
            auto end = Clock::now();

            results.find += elapsed(start, end);
        }

        // Rank queries
        {
            Treap<int, int> treap;
            for (int key : randomKeys) {
                treap.insert(key, key);
            }

            volatile std::int64_t checksum = 0;

            auto start = Clock::now();
            for (std::size_t index : rankQueries) {
                checksum += treap.rank(index);
            }
            auto end = Clock::now();

            results.rank += elapsed(start, end);
        }

        // Remove all keys
        {
            Treap<int, int> treap;
            for (int key : randomKeys) {
                treap.insert(key, key);
            }

            auto start = Clock::now();
            for (int key : randomKeys) {
                treap.remove(key);
            }
            auto end = Clock::now();

            results.remove += elapsed(start, end);
        }

        // Repeated size() calls
        {
            Treap<int, int> treap;
            for (int key : randomKeys) {
                treap.insert(key, key);
            }

            const std::size_t calls = 1000000;
            volatile std::size_t checksum = 0;

            auto start = Clock::now();
            for (std::size_t i = 0; i < calls; ++i) {
                checksum += treap.size();
            }
            auto end = Clock::now();

            results.size += elapsed(start, end) * 1000.0 / calls;
        }
    }

    results.insertRandom /= repetitions;
    results.insertSorted /= repetitions;
    results.find /= repetitions;
    results.rank /= repetitions;
    results.remove /= repetitions;
    results.size /= repetitions;

    return results;
}

int main() {
    const std::vector<std::size_t> sizes = {
        1000, 5000, 10000, 50000, 100000, 500000
    };

    const int repetitions = 5;
    std::mt19937_64 rng(std::random_device{}());

    std::cout << std::fixed << std::setprecision(3);

    std::cout << std::left
              << std::setw(12) << "N"
              << std::setw(18) << "Insert Random"
              << std::setw(18) << "Insert Sorted"
              << std::setw(15) << "Find"
              << std::setw(15) << "Rank"
              << std::setw(15) << "Remove"
              << "Size (ns/call)\n";

    std::cout << std::string(108, '-') << '\n';

    for (std::size_t n : sizes) {
        Results result = benchmark(n, repetitions, rng);

        std::cout << std::left
                  << std::setw(12) << n
                  << std::setw(18) << result.insertRandom
                  << std::setw(18) << result.insertSorted
                  << std::setw(15) << result.find
                  << std::setw(15) << result.rank
                  << std::setw(15) << result.remove
                  << result.size << '\n';
    }

    std::cout << "\nAll times are in milliseconds, except size (ns/call).\n";
    std::cout << "Results averaged over " << repetitions << " runs.\n";

    return 0;
}
