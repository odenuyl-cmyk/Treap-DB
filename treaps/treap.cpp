#include "treap.h"

template<typename K, typename V>
void Treap<K,V>::insert(K key, V value) {
    Node* newNode = new Node{key, value, generate_priority(), nullptr, nullptr}
}