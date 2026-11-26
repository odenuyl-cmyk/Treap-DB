#include <iostream>
#include <map>
#include <random>
#include <string>
#include <stdexcept>
#include <ctime>

#include "treaps/treap.h"

bool verify(Treap<int, std::string>& treap, const std::map<int, std::string>& reference, int step) {
    if (treap.size() != reference.size()) {
        std::cerr << "FAIL at step " << step
                  << ": size mismatch (treap=" << treap.size()
                  << ", map=" << reference.size() << ")\n";
        return false;
    }

    std::size_t rank = 0;

    // check every stored key/value pair and its sorted rank.
    for (auto it = reference.begin(); it != reference.end(); ++it) {
        int key = it->first;
        const std::string& expectedValue = it->second;

        std::string* actualValue = treap.find(key);

        if (actualValue == nullptr || *actualValue != expectedValue) {
            std::cerr << "FAIL at step " << step
                      << ": incorrect or missing value for key " << key << "\n";
            return false;
        }

        try {
            int rankedKey = treap.rank(rank);
            if (rankedKey != key) {
                std::cerr << "FAIL at step " << step
                          << ": rank(" << rank << ") returned " << rankedKey
                          << ", expected " << key << "\n";
                return false;
            }
        } catch (const std::exception& e) {
            std::cerr << "FAIL at step " << step
                      << ": rank(" << rank << ") threw: " << e.what() << "\n";
            return false;
        }

        ++rank;
    }

    // rank(size) must be out of bounds, even for an empty treap.
    try {
        treap.rank(reference.size());
        std::cerr << "FAIL at step " << step
                  << ": rank(size) did not throw\n";
        return false;
    } catch (const std::out_of_range&) {
        // expected exception
    } catch (const std::exception& e) {
        std::cerr << "FAIL at step " << step
                  << ": rank(size) threw the wrong exception: " << e.what() << "\n";
        return false;
    }

    return true;
}

int main() {
    Treap<int, std::string> treap;
    std::map<int, std::string> reference;

    std::mt19937_64 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<int> keyDist(-5000, 5000);
    std::uniform_int_distribution<int> operationDist(0, 99);

    const int NUM_OPERATIONS = 100000;
    const int VERIFY_EVERY = 100;
    int valueCounter = 0;

    for (int step = 1; step <= NUM_OPERATIONS; ++step) {
        int key = keyDist(rng);
        int operation = operationDist(rng);

        if (operation < 65) {
            // insert, including updates to existing keys
            std::string value = "value_" + std::to_string(valueCounter++);
            treap.insert(key, value);
            reference[key] = value;
        } else if (operation < 75) {
            // remove, including keys that may not exist
            treap.remove(key);
            reference.erase(key);
        } else {
            // compare successful and unsuccessful lookups
            std::string* actual = treap.find(key);
            std::map<int, std::string>::const_iterator expected = reference.find(key);

            if (expected == reference.end()) {
                if (actual != nullptr) {
                    std::cerr << "FAIL at step " << step
                              << ": find(" << key << ") should return nullptr\n";
                    return 1;
                }
            } else {
                if (actual == nullptr || *actual != expected->second) {
                    std::cerr << "FAIL at step " << step
                              << ": find(" << key << ") returned an incorrect value\n";
                    return 1;
                }
            }
        }

        if (step % VERIFY_EVERY == 0) {
            if (!verify(treap, reference, step)) {
                return 1;
            }
        }
    }

    if (!verify(treap, reference, NUM_OPERATIONS)) {
        return 1;
    }

    std::cout << "ALL TESTS PASSED\n";
    std::cout << "Operations performed: " << NUM_OPERATIONS << "\n";
    std::cout << "Final number of keys: " << treap.size() << "\n";
    std::cout << "Reference container: std::map<int, std::string>\n";

    return 0;
}
