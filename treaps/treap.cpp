#include "treap.h"
#include <stdexcept>

// updates the size value of a given node
template<typename K, typename V>
void Treap<K,V>::update(Node* node) {
    if (node == nullptr) return;

    size_t leftSize = node->left ? node->left->size : 0;
    size_t rightSize = node->right ? node->right->size : 0;

    node->size = 1 + leftSize + rightSize;
}

// finds the value for a given key in the Treap, or null if the key does not exist
template<typename K, typename V>
V* Treap<K,V>::find(K key) {
    Node* currNode = root;

    while (currNode != nullptr) {
        if (currNode->key < key) {
            currNode = currNode->right;
        } else if (currNode->key > key) {
            currNode = currNode->left;
        } else {
            return &currNode->value;
        }
    }

    return nullptr;
}

// splits a tree based on values less than and greater than or equal a given key
template<typename K, typename V>
void Treap<K,V>::split(Node* node, K key, Node*& L, Node*& R) {
    if (node == nullptr) {
        L = nullptr;
        R = nullptr;
        return;
    }

    if (node->key < key) {
        L = node;
        split(node->right, key, L->right, R);
        update(L);
    } else {
        R = node;
        split(node->left, key, L, R->left);
        update(R);
    }
}

// inserts a new node in the correct place in an existing tree
template<typename K, typename V>
typename Treap<K,V>::Node* Treap<K,V>::insert(Node* node, Node* newNode) {
    if (node == nullptr) {
        return newNode;
    }

    if (newNode->priority > node->priority) {
        split(node, newNode->key, newNode->left, newNode->right);
        update(newNode);
        return newNode;
    }

    if (newNode->key < node->key) {
        node->left = insert(node->left, newNode);
    } else if (newNode->key > node->key) {
        node->right = insert(node->right, newNode);
    } else {
        node->value = newNode->value;
        delete newNode;
    }

    update(node);
    return node;
}

// public interface for inserting a key-value pair
template<typename K, typename V>
void Treap<K,V>::insert(K key, V value) {
    V* existing = find(key);
    if (existing != nullptr) {
        *existing = value;
        return;
    }

    Node* newNode = new Node{key, value, generate_priority(), nullptr, nullptr, 1};
    root = insert(root, newNode);
}

// returns the stored size value of the Treap
template<typename K, typename V>
size_t Treap<K,V>::size() {
    return root ? root->size : 0;
}

// merges two trees together, where all keys in L are strictly less than all keys in R
template<typename K, typename V>
typename Treap<K,V>::Node* Treap<K,V>::merge(Node* L, Node* R) {
    if (L == nullptr) return R;
    if (R == nullptr) return L;

    if (L->priority > R->priority) {
        L->right = merge(L->right, R);
        update(L);
        return L;
    } else {
        R->left = merge(L, R->left);
        update(R);
        return R;
    }
}

// removes a key-value pair from a tree
template<typename K, typename V>
typename Treap<K,V>::Node* Treap<K,V>::remove(Node* node, K key) {
    if (node == nullptr) return nullptr;

    if (key < node->key) {
        node->left = remove(node->left, key);
    } else if (key > node->key) {
        node->right = remove(node->right, key);
    } else {
        Node* merged = merge(node->left, node->right);
        delete node;
        return merged;
    }

    update(node);
    return node;
}

// public interface for removing a key-value pair
template<typename K, typename V>
void Treap<K,V>::remove(K key) {
    root = remove(root, key);
}

// returns the key of rank r within the Treap
template<typename K, typename V>
K Treap<K,V>::rank(size_t r) {
    Node* currNode = root;

    if (r >= size()) {
        throw std::out_of_range("rank out of bounds");
    }

    while (currNode != nullptr) {
        size_t leftSize = currNode->left ? currNode->left->size : 0;

        if (r < leftSize) {
            currNode = currNode->left;
        } else if (r == leftSize) {
            return currNode->key;
        } else {
            r -= leftSize + 1;
            currNode = currNode->right;
        }
    }

    throw std::out_of_range("rank out of bounds");
}

template<typename K, typename V>
void Treap<K,V>::destroy(Node* node) {
    if (node == nullptr) return;

    destroy(node->left);
    destroy(node->right);

    delete node;
}

template<typename K, typename V>
Treap<K,V>::~Treap() {
    destroy(root);
}