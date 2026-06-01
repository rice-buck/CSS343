// test_hashtable.cpp
// Tests for the HashTable<K, V> class. Compile with:
//   g++ -std=c++17 -Wall test_hashtable.cpp -o test_hashtable

#include <cassert>
#include <sstream>
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

// ---------- print ----------
static void test_print() {
    // Empty table: header should reflect size=0, all buckets empty.
    {
        HashTable<std::string, int> t;
        std::ostringstream oss;
        t.print(oss);
        std::string out = oss.str();

        assert(out.find("size=0")        != std::string::npos);
        assert(out.find("capacity=16")   != std::string::npos);
        assert(out.find("16 empty")      != std::string::npos);
    }

    // Non-empty table: every key and value should appear somewhere.
    {
        HashTable<std::string, int> t;
        t.insert("apple",  1);
        t.insert("banana", 2);
        t.insert("cherry", 3);

        std::ostringstream oss;
        t.print(oss);
        std::string out = oss.str();

        assert(out.find("size=3") != std::string::npos);
        assert(out.find("apple")  != std::string::npos);
        assert(out.find("banana") != std::string::npos);
        assert(out.find("cherry") != std::string::npos);
        assert(out.find("=1")     != std::string::npos);
        assert(out.find("=2")     != std::string::npos);
        assert(out.find("=3")     != std::string::npos);
    }

    // Collision rendering: force two keys into the same bucket so the
    // chain "->" arrow appears. std::hash<int> is effectively the identity
    // on common stdlibs, so hash(0) % 4 == hash(4) % 4 == 0 deterministically.
    {
        HashTable<int, std::string> t(4);
        t.insert(0, "zero");
        t.insert(4, "four");

        std::ostringstream oss;
        t.print(oss);
        std::string out = oss.str();

        assert(out.find("zero") != std::string::npos);
        assert(out.find("four") != std::string::npos);
        assert(out.find("->")   != std::string::npos);   // chain arrow
    }

    // Verify the function honors the passed stream — if it wrote to cout
    // instead, the ostringstream would be empty.
    {
        HashTable<std::string, int> t;
        t.insert("test", 42);

        std::ostringstream oss;
        t.print(oss);
        assert(!oss.str().empty());
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
    test_print();                std::cout << "  print             OK\n";
    std::cout << "All tests passed.\n";
    return 0;
}