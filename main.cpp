
#include <iostream>
#include <string>
#include "treaps/treap.h"

int main() {
    Treap<int, std::string> db;

    int choice;

    while (true) {
        std::cout << "\n===== TREAP DATABASE =====\n";
        std::cout << "1. Insert\n";
        std::cout << "2. Find\n";
        std::cout << "3. Remove\n";
        std::cout << "4. Size\n";
        std::cout << "5. Rank\n";
        std::cout << "6. Exit\n";
        std::cout << "Choice: ";

        std::cin >> choice;

        if (choice == 1) {
            int key;
            std::string value;

            std::cout << "Key: ";
            std::cin >> key;

            std::cout << "Value: ";
            std::cin >> value;

            db.insert(key, value);

            std::cout << "Inserted (" << key << ", " << value << ")\n";
        }
        else if (choice == 2) {
            int key;

            std::cout << "Key: ";
            std::cin >> key;

            std::string* value = db.find(key);

            if (value != nullptr) {
                std::cout << "Found: " << *value << "\n";
            } else {
                std::cout << "Key not found.\n";
            }
        }
        else if (choice == 3) {
            int key;

            std::cout << "Key: ";
            std::cin >> key;

            db.remove(key);

            std::cout << "Removed key (if it existed).\n";
        }
        else if (choice == 4) {
            std::cout << "Database size: " << db.size() << "\n";
        }
        else if (choice == 5) {
            size_t r;

            std::cout << "Rank (0-indexed): ";
            std::cin >> r;

            try {
                std::cout << "Key at rank " << r << ": "
                          << db.rank(r) << "\n";
            } catch (const std::out_of_range& e) {
                std::cout << e.what() << "\n";
            }
        }
        else if (choice == 6) {
            break;
        }
        else {
            std::cout << "Invalid choice.\n";
        }
    }

    return 0;
}
