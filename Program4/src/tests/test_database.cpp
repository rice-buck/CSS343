// Unit tests for Database — focused on the behaviors the original code was
// missing: stock checks, unknown-customer/movie checks, and the
// "never borrowed" return check. Also verifies content-keyed lookup survives
// polymorphic storage (no slicing).
//
// Database/Customer report errors via stderr and silently discard the
// command (per spec: "discard the command, display an error message"), so
// these tests assert on *state* (stock, history size) rather than output text.
#include <cassert>
#include <iostream>
#include <sstream>

#include "../Database.h"
#include "../Customer.h"
#include "../Comedy.h"
#include "../Drama.h"
#include "../Classic.h"

static void test_add_and_lookup_by_content_key() {
    Database db;
    Comedy* c = new Comedy('F', 10, "Fargo", "Joel Coen", 1996);
    const std::string key = c->genHashKey();
    db.addMovie(c);                       // db now owns c

    db.addCustomer(Customer(1, "Joe", "Mama"));
    db.borrowMovie(1, key);               // should succeed silently

    Customer* cust = db.customers.find(1);
    assert(cust != nullptr);
    assert(cust->hasBorrowed(key));
    assert(cust->getHistory().size() == 1);
}

static void test_borrow_decrements_and_return_increments_stock() {
    Database db;
    Comedy* c = new Comedy('F', 1, "Annie Hall", "Woody Allen", 1977);
    const std::string key = c->genHashKey();
    db.addMovie(c);
    db.addCustomer(Customer(1, "Joe", "Mama"));

    db.borrowMovie(1, key);
    assert(c->getStock() == 0);

    db.returnMovie(1, key);
    assert(c->getStock() == 1);
}

static void test_borrow_rejected_when_out_of_stock() {
    Database db;
    Comedy* c = new Comedy('F', 0, "Empty Shelf", "Nobody", 2000);
    const std::string key = c->genHashKey();
    db.addMovie(c);
    db.addCustomer(Customer(1, "Joe", "Mama"));

    db.borrowMovie(1, key);
    assert(c->getStock() == 0);                       // unchanged
    assert(!db.customers.find(1)->hasBorrowed(key));  // not recorded
    assert(db.customers.find(1)->getHistory().empty());
}

static void test_borrow_rejected_for_unknown_customer() {
    Database db;
    Comedy* c = new Comedy('F', 5, "Some Movie", "Some Director", 2001);
    const std::string key = c->genHashKey();
    db.addMovie(c);
    // No customer added.
    db.borrowMovie(9999, key);
    assert(c->getStock() == 5);   // unchanged — command discarded
}

static void test_borrow_rejected_for_unknown_movie() {
    Database db;
    db.addCustomer(Customer(1, "Joe", "Mama"));
    db.borrowMovie(1, "F:Does Not Exist:1900");
    assert(db.customers.find(1)->getHistory().empty());
}

static void test_return_rejected_when_never_borrowed() {
    Database db;
    Comedy* c = new Comedy('F', 5, "Untouched", "Director", 2002);
    const std::string key = c->genHashKey();
    db.addMovie(c);
    db.addCustomer(Customer(1, "Joe", "Mama"));

    db.returnMovie(1, key);
    assert(c->getStock() == 5);                              // unchanged
    assert(db.customers.find(1)->getHistory().empty());      // no transaction recorded
}

static void test_classic_editions_tracked_independently() {
    Database db;
    Classic* hepburn = new Classic('C', 10, "Holiday", "George Cukor",
                                   "Katherine Hepburn", 9, 1938);
    Classic* grant   = new Classic('C', 10, "Holiday", "George Cukor",
                                   "Cary Grant", 9, 1938);
    const std::string hepburnKey = hepburn->genHashKey();
    const std::string grantKey   = grant->genHashKey();
    db.addMovie(hepburn);
    db.addMovie(grant);
    db.addCustomer(Customer(1, "Joe", "Mama"));

    db.borrowMovie(1, hepburnKey);
    assert(hepburn->getStock() == 9);
    assert(grant->getStock() == 10);          // untouched — separate edition
}

static void test_history_records_most_recent_first() {
    Database db;
    Comedy* a = new Comedy('F', 5, "Movie A", "Dir A", 2000);
    Comedy* b = new Comedy('F', 5, "Movie B", "Dir B", 2001);
    const std::string keyA = a->genHashKey();
    const std::string keyB = b->genHashKey();
    db.addMovie(a);
    db.addMovie(b);
    db.addCustomer(Customer(1, "Joe", "Mama"));

    db.borrowMovie(1, keyA);
    db.borrowMovie(1, keyB);
    db.returnMovie(1, keyA);

    auto hist = db.customers.find(1)->getHistory();
    assert(hist.size() == 3);
    // Most recent transaction (the return of A) must be on top.
    assert(hist.top().find("Returned") != std::string::npos);
    assert(hist.top().find("Movie A") != std::string::npos);
}

int main() {
    test_add_and_lookup_by_content_key();
        std::cout << "  add + content-key lookup        OK\n";
    test_borrow_decrements_and_return_increments_stock();
        std::cout << "  borrow/return adjust stock      OK\n";
    test_borrow_rejected_when_out_of_stock();
        std::cout << "  borrow rejected: stock 0        OK\n";
    test_borrow_rejected_for_unknown_customer();
        std::cout << "  borrow rejected: bad customer   OK\n";
    test_borrow_rejected_for_unknown_movie();
        std::cout << "  borrow rejected: bad movie      OK\n";
    test_return_rejected_when_never_borrowed();
        std::cout << "  return rejected: never borrowed OK\n";
    test_classic_editions_tracked_independently();
        std::cout << "  classic editions independent    OK\n";
    test_history_records_most_recent_first();
        std::cout << "  history newest-first ordering   OK\n";
    std::cout << "All Database tests passed.\n";
    return 0;
}
