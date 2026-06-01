#pragma once

template <typename K, typename V>
struct Node {
    K key;   // stores node key
    V value;     // stores node value
    Node* next;    // pointer to next node

    // Constructor initializes node value and sets next to null
    Node(const K& k, const V& v) : key(k), value(v), next(nullptr) {};
};