#pragma once

#include <cstdint>
#include <cstddef>
#include <random>

template <typename K, typename V> 
class Treap {
    private:
        Node* root = nullptr;
        size_t tree_size;

        void update(Node* node);
        Node* join_mid(Node* L, Node* mid, Node* R);

        std::mt19937_64 rng{std::random_device{}()};
        uint64_t generate_priority() {
            return rng();
        }
    
    public:
        struct Node {
            K key;
            V value;
            uint64_t priority;

            Node* left;
            Node* right;
        };

        size_t size();
        
        void insert(K key, V value);
        void remove(K key);
        V* find(K key);
        K rank(int r);
};