#pragma once

#include <cstdint>
#include <cstddef>
#include <random>

template <typename K, typename V> 
class Treap {
    private:
        struct Node {
            K key;
            V value;
            uint64_t priority;

            Node* left;
            Node* right;

            size_t size;
        }; 

        Node* root = nullptr;

        void update(Node* node);
        Node* insert(Node* node, Node* newNode);
        Node* remove(Node* node, K key);
        void split(Node* node, K key, Node*& L, Node*& R);
        Node* merge(Node* L, Node* R);

        std::mt19937_64 rng{std::random_device{}()};
        uint64_t generate_priority() {
            return rng();
        }

        void destroy(Node* node);
    
    public:
        ~Treap();
        Treap(const Treap&) = delete;
        Treap& operator=(const Treap&) = delete;

        size_t size();
        
        void insert(K key, V value);
        void remove(K key);
        
        V* find(K key);
        K rank(size_t r);
};

#include "treap.cpp"