#pragma once
#include <string>
#include <vector>

#include "HashTable.h"
#include "Movie.h"
#include "Customer.h"

// The central controller. Owns the inventory and customer registries and
// provides one method per supported operation (spec: Database.h).
//
// Movies are looked up by a content-based string key (genHashKey()), not a
// sequential ID — that's what lets a "B 1234 D C 9 1938 Katherine Hepburn"
// command resolve to the correct inventory entry. The hash table stores
// non-owning Movie* so that Comedy/Drama/Classic keep their full identity
// (no slicing); `movieStore` is the sole owner and frees them on destruction.
class Database {
public:
    HashTable<int, Customer> customers;

    Database();
    ~Database();
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    // Takes ownership of newMovie (heap-allocated by the caller).
    void addMovie(Movie* newMovie);

    void addCustomer(const Customer& newCust);

    void borrowMovie(int custID, const std::string& movieKey);
    void returnMovie(int custID, const std::string& movieKey);

    void inventory() const;
    void history(int custID);

private:
    HashTable<std::string, Movie*> movies;
    std::vector<Movie*> movieStore;   // owns every Movie*

    static std::string transactionLabel(const Movie* m, const char* verb);
};
