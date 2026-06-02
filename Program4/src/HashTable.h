#pragma once
#include <iostream>
#include <vector>
#include <cstddef>
#include <functional>
#include "Node.h"

template <typename K, typename V>
class HashTable {

  private:
    std::vector<Node<K, V>*> buckets_;
    size_t capacity_;
    size_t size_;

    // Helper to map key to index
    size_t hash(const K& key) const {
      return std::hash<K>{}(key) % capacity_;
    }

    void swap(HashTable& other) noexcept;

  public:
    // Constructor - Initialize buckets with nullptr
    explicit HashTable(size_t capacity = 16);

    // Destructor
    ~HashTable();

    // Copy constructor
    HashTable(const HashTable& other);

    // Pass by value copy
    HashTable& operator=(HashTable other);

    // Insert item into table
    bool insert(const K& key, const V& value);

    // Remove value from table
    bool remove(const K& key);

    // Search for key (const version)
    const V* find(const K& key) const;

    // Search for key (non-const version)
    V* find(const K& key);

    // Checks if table contains value
    bool contains(const K& key) const { return find(key) != nullptr; }

    size_t getSize()     const { return size_; }
    size_t getCapacity() const { return capacity_; }
    bool   empty()       const { return size_ == 0; }

    // Visits every entry in table
    void forEach(std::function<void(const K&, V&)> fn);

    // Rebuilds table with new capacity
    void rehash(size_t newCapacity);

    // Print table
    void print(std::ostream& os = std::cout) const;
};

// ----- implementations  -----

template <typename K, typename V>
HashTable<K, V>::HashTable(size_t capacity)
    : buckets_(capacity, nullptr), capacity_(capacity), size_(0) {}

template <typename K, typename V>
HashTable<K, V>::~HashTable() {
    for (Node<K, V>* head : buckets_) {
        while (head) {
            Node<K, V>* tmp = head;
            head = head->next;
            delete tmp;
        }
    }
}

template <typename K, typename V>
HashTable<K, V>::HashTable(const HashTable& other)
    : buckets_(other.capacity_, nullptr),
      capacity_(other.capacity_),
      size_(other.size_) {
    // Same capacity + same hash function => every node lands in its original
    // bucket index, so we can skip recomputing the hash entirely.
    for (size_t i = 0; i < other.capacity_; ++i) {
        for (Node<K, V>* n = other.buckets_[i]; n; n = n->next) {
            Node<K, V>* copy = new Node<K, V>(n->key, n->value);
            copy->next = buckets_[i];
            buckets_[i] = copy;
        }
    }
}

template <typename K, typename V>
HashTable<K, V>& HashTable<K, V>::operator=(HashTable other) {
    // 'other' was copy-constructed from the rhs (pass by value).
    // Swap with it, then 'other' destructor frees our old data on the way out.
    // Handles self-assignment automatically and is exception-safe.
    swap(other);
    return *this;
}

template <typename K, typename V>
void HashTable<K, V>::swap(HashTable& other) noexcept {
    using std::swap;
    swap(buckets_,  other.buckets_);
    swap(capacity_, other.capacity_);
    swap(size_,     other.size_);
}

template <typename K, typename V>
bool HashTable<K, V>::insert(const K& key, const V& value) {
    size_t idx = hash(key);
    // If the key already exists in this chain, update and return false.
    for (Node<K, V>* n = buckets_[idx]; n; n = n->next) {
        if (n->key == key) { n->value = value; return false; }
    }
    // Otherwise prepend a new node (O(1)).
    Node<K, V>* node = new Node<K, V>(key, value);
    node->next = buckets_[idx];
    buckets_[idx] = node;
    ++size_;

    // Auto-rehash at load factor > 0.75. Integer form avoids floating point.
    if (size_ * 4 > capacity_ * 3) {
        rehash(capacity_ * 2);
    }
    return true;
}

template <typename K, typename V>
bool HashTable<K, V>::remove(const K& key) {
    size_t idx = hash(key);
    Node<K, V>* prev = nullptr;
    Node<K, V>* cur  = buckets_[idx];
    while (cur) {
        if (cur->key == key) {
            if (prev) prev->next = cur->next;
            else      buckets_[idx] = cur->next;
            delete cur;
            --size_;
            return true;
        }
        prev = cur;
        cur  = cur->next;
    }
    return false;
}

template <typename K, typename V>
const V* HashTable<K, V>::find(const K& key) const {
    for (const Node<K, V>* n = buckets_[hash(key)]; n; n = n->next) {
        if (n->key == key) return &n->value;
    }
    return nullptr;
}

template <typename K, typename V>
V* HashTable<K, V>::find(const K& key) {
    for (Node<K, V>* n = buckets_[hash(key)]; n; n = n->next) {
        if (n->key == key) return &n->value;
    }
    return nullptr;
}



template <typename K, typename V>
void HashTable<K, V>::forEach(std::function<void(const K&, V&)> fn) {
    for (Node<K, V>* head : buckets_) {
        for (Node<K, V>* n = head; n; n = n->next) {
            fn(n->key, n->value);
        }
    }
}

template <typename K, typename V>
void HashTable<K, V>::print(std::ostream& os) const {
    os << "HashTable [size=" << size_
       << ", capacity=" << capacity_ << "]\n";

    size_t emptyBuckets = 0;
    for (size_t i = 0; i < capacity_; ++i) {
        if (!buckets_[i]) { ++emptyBuckets; continue; }

        os << "  [" << i << "] ";
        for (const Node<K, V>* n = buckets_[i]; n; n = n->next) {
            os << n->key << "=" << n->value;
            if (n->next) os << " -> ";
        }
        os << "\n";
    }
    if (emptyBuckets > 0) {
        os << "  (" << emptyBuckets << " empty bucket"
           << (emptyBuckets == 1 ? "" : "s") << ")\n";
    }
}

template <typename K, typename V>
void HashTable<K, V>::rehash(size_t newCapacity) {
    if (newCapacity == 0) newCapacity = 1;
    std::vector<Node<K, V>*> newBuckets(newCapacity, nullptr);

    // Move existing nodes into the new array — no Node allocations happen.
    // Caller-held pointers to a Node's value field remain valid across rehash.
    for (size_t i = 0; i < capacity_; ++i) {
        Node<K, V>* n = buckets_[i];
        while (n) {
            Node<K, V>* next = n->next;
            size_t idx = std::hash<K>{}(n->key) % newCapacity;
            n->next = newBuckets[idx];
            newBuckets[idx] = n;
            n = next;
        }
    }
    buckets_  = std::move(newBuckets);
    capacity_ = newCapacity;
}