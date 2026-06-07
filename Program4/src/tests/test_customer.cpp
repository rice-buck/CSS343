// Unit tests for Customer, including the borrowed-key tracking that backs
// the "return item never borrowed" error check in Database::returnMovie.
#include <cassert>
#include <iostream>

#include "../Customer.h"

static void test_basic_accessors() {
    Customer c(1234, "Mickey", "Mouse");
    assert(c.getID() == 1234);
    assert(c.getFName() == "Mickey");
    assert(c.getLName() == "Mouse");

    c.setID(5678);
    assert(c.getID() == 5678);
}

static void test_history_is_lifo() {
    Customer c(1, "Joe", "Mama");
    c.addTransaction("first");
    c.addTransaction("second");
    c.addTransaction("third");

    auto hist = c.getHistory();
    assert(hist.top() == "third");  hist.pop();
    assert(hist.top() == "second"); hist.pop();
    assert(hist.top() == "first");  hist.pop();
    assert(hist.empty());
}

static void test_borrow_tracking_round_trip() {
    Customer c(1, "Joe", "Mama");
    const std::string key = "F:Title:2000";

    assert(!c.hasBorrowed(key));
    c.recordBorrow(key);
    assert(c.hasBorrowed(key));

    // Successful return clears the outstanding borrow.
    assert(c.recordReturn(key) == true);
    assert(!c.hasBorrowed(key));
}

static void test_return_without_borrow_fails() {
    Customer c(1, "Joe", "Mama");
    // Never borrowed this — recordReturn must report failure and not crash.
    assert(c.recordReturn("F:Never:1999") == false);
    assert(!c.hasBorrowed("F:Never:1999"));
}

static void test_double_return_fails_second_time() {
    Customer c(1, "Joe", "Mama");
    const std::string key = "D:Director:Title";
    c.recordBorrow(key);
    assert(c.recordReturn(key) == true);
    assert(c.recordReturn(key) == false);   // already returned — can't again
}

int main() {
    test_basic_accessors();              std::cout << "  basic accessors            OK\n";
    test_history_is_lifo();              std::cout << "  history is LIFO            OK\n";
    test_borrow_tracking_round_trip();   std::cout << "  borrow/return round trip   OK\n";
    test_return_without_borrow_fails();  std::cout << "  return w/o borrow fails    OK\n";
    test_double_return_fails_second_time();
                                         std::cout << "  double return rejected     OK\n";
    std::cout << "All Customer tests passed.\n";
    return 0;
}
