#include "treap.h"

template<typename K, typename V>
void Treap<K,V>::join_mid(Treap* L, K key, V value, Treap* R) {
    return;
}

template<typename K, typename V>
void Treap<K,V>::insert(K key, V value) {
    Node* newNode = new Node{key, value, generate_priority(), nullptr, nullptr};
}

template<typename K, typename V>
size_t Treap<K,V>::size() {
    return tree_size;
}

template<typename K, typename V>
V* Treap<K,V>::find(K key) {
    Node* currNode = root;

    while (currNode != nullptr) {
        if (currNode->key < key) {
            currNode = currNode->right;
        } else if (currNode->key > k) {
            currNode = currNode->left;
        } else {
            return &currNode->value
        }
    }

    return nullptr;
}

template<typename K, typename V>
void Treap<K,V>::insert(K key, V value) {
    Node* currNode = root;

    while (currNode != nullptr) {
        if (currNode->key < key) {
            currNode = currNode->right;
        } else if (currNode->key > k) {
            currNode = currNode->left;
        } else {
            return &currNode->value
        }
    }

    return join_mid();
}