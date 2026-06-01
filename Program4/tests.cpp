// test_hashtable.cpp
// Tests for the HashTable<K, V> class. Compile with:
//   g++ -std=c++17 -Wall test_hashtable.cpp -o test_hashtable

#include <cassert>
#include <iostream>
#include <string>
#include "HashTable.h"

// ---------- basic insert / find ----------
static void test_insert_and_find() {
    HashTable<std::string, int> t;
    assert(t.empty());
    assert(t.getSize() == 0);

    assert(t.insert("apple",  1) == true);
    assert(t.insert("banana", 2) == true);
    assert(t.insert("cherry", 3) == true);

    assert(t.getSize() == 3);
    assert(!t.empty());

    assert(*t.find("apple")  == 1);
    assert(*t.find("banana") == 2);
    assert(*t.find("cherry") == 3);
    assert(t.find("missing") == nullptr);
}

// ---------- updating an existing key ----------
static void test_update_existing_key() {
    HashTable<std::string, int> t;
    t.insert("apple", 1);
    // Second insert with same key should update, not add a new entry.
    assert(t.insert("apple", 99) == false);
    assert(t.getSize() == 1);
    assert(*t.find("apple") == 99);
}

// ---------- contains ----------
static void test_contains() {
    HashTable<std::string, int> t;
    t.insert("x", 10);
    assert(t.contains("x"));
    assert(!t.contains("y"));
}

// ---------- remove ----------
static void test_remove() {
    HashTable<std::string, int> t;
    t.insert("a", 1);
    t.insert("b", 2);
    t.insert("c", 3);

    assert(t.remove("b") == true);
    assert(t.getSize() == 2);
    assert(t.find("b") == nullptr);
    assert(*t.find("a") == 1);
    assert(*t.find("c") == 3);

    // Removing something that isn't there should return false, leave size alone.
    assert(t.remove("zzz") == false);
    assert(t.getSize() == 2);
}

// ---------- copy constructor (deep copy, independence) ----------
static void test_copy_constructor() {
    HashTable<std::string, int> a;
    a.insert("one", 1);
    a.insert("two", 2);

    HashTable<std::string, int> b = a;        // copy construct
    assert(b.getSize() == 2);
    assert(*b.find("one") == 1);

    // Modifying the copy must not affect the original.
    b.insert("three", 3);
    assert(b.getSize() == 3);
    assert(a.getSize() == 2);
    assert(a.find("three") == nullptr);
}

// ---------- copy assignment ----------
static void test_copy_assignment() {
    HashTable<std::string, int> a;
    a.insert("foo", 42);

    HashTable<std::string, int> b;
    b.insert("temp", 99);
    b = a;                                     // copy assign (overwrites b)

    assert(b.getSize() == 1);
    assert(*b.find("foo") == 42);
    assert(b.find("temp") == nullptr);

    // Self-assignment should be a no-op (copy-and-swap handles it).
    a = a;
    assert(a.getSize() == 1);
    assert(*a.find("foo") == 42);
}

// ---------- forEach ----------
static void test_forEach() {
    HashTable<std::string, int> t;
    t.insert("a", 1);
    t.insert("b", 2);
    t.insert("c", 3);

    // forEach can read every entry.
    int sum = 0;
    t.forEach([&sum](const std::string& /*k*/, int& v) { sum += v; });
    assert(sum == 6);

    // forEach can mutate values in place.
    t.forEach([](const std::string& /*k*/, int& v) { v *= 10; });
    assert(*t.find("a") == 10);
    assert(*t.find("b") == 20);
    assert(*t.find("c") == 30);
}

// ---------- manual rehash ----------
static void test_manual_rehash() {
    HashTable<std::string, int> t(4);
    t.insert("a", 1);
    t.insert("b", 2);
    t.insert("c", 3);

    t.rehash(32);
    assert(t.getCapacity() == 32);
    assert(t.getSize() == 3);                  // size unchanged
    assert(*t.find("a") == 1);                 // entries still reachable
    assert(*t.find("b") == 2);
    assert(*t.find("c") == 3);
}

// ---------- auto-rehash triggered by load factor ----------
static void test_auto_rehash() {
    HashTable<int, int> t(4);
    size_t initialCap = t.getCapacity();

    for (int i = 0; i < 100; ++i) t.insert(i, i * i);

    assert(t.getSize() == 100);
    assert(t.getCapacity() > initialCap);      // grew at least once

    for (int i = 0; i < 100; ++i) {
        const int* v = t.find(i);
        assert(v && *v == i * i);
    }
}

int main() {
    test_insert_and_find();      std::cout << "  insert/find       OK\n";
    test_update_existing_key();  std::cout << "  update key        OK\n";
    test_contains();             std::cout << "  contains          OK\n";
    test_remove();               std::cout << "  remove            OK\n";
    test_copy_constructor();     std::cout << "  copy constructor  OK\n";
    test_copy_assignment();      std::cout << "  copy assignment   OK\n";
    test_forEach();              std::cout << "  forEach           OK\n";
    test_manual_rehash();        std::cout << "  manual rehash     OK\n";
    test_auto_rehash();          std::cout << "  auto rehash       OK\n";
    std::cout << "All tests passed.\n";
    return 0;
}