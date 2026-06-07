// Culminating end-to-end test: drives Operations against the real
// test-cases/data4{movies,customers,commands}.txt files (the same files
// main.cpp loads by default) and asserts on the resulting state.
//
// The expected values below come from a hand trace of all 59 lines of
// data4commands.txt against the 30-line catalog and 12-customer roster:
// every B/R was followed forward, tallying stock and per-customer borrow
// counts, to determine which commands succeed, which are rejected (and why),
// and what each customer's final history size is.
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

#include "../Database.h"
#include "../Operations.h"

namespace {

template <typename Fn>
std::string captureOutput(std::ostream& stream, Fn&& fn) {
    std::ostringstream buf;
    std::streambuf* old = stream.rdbuf(buf.rdbuf());
    fn();
    stream.rdbuf(old);
    return buf.str();
}

const std::string kMoviesFile    = "test-cases/data4movies.txt";
const std::string kCustomersFile = "test-cases/data4customers.txt";
const std::string kCommandsFile  = "test-cases/data4commands.txt";

} // namespace

int main() {
    Database db;
    Operations ops;

    // Lines 16 and 24 of data4movies.txt use the unsupported code 'Z' — both
    // must be reported and skipped, leaving 28 valid catalog entries.
    std::string loadErr = captureOutput(std::cerr, [&] {
        ops.readMovies(kMoviesFile, db);
        ops.readCustomers(kCustomersFile, db);
    });
    assert(loadErr.find("invalid movie code") != std::string::npos);

    std::string out;
    std::string cmdErr = captureOutput(std::cerr, [&] {
        out = captureOutput(std::cout, [&] { ops.readCommands(kCommandsFile, db); });
    });

    // ---- Every error path the command script is designed to exercise ----
    assert(cmdErr.find("invalid action code") != std::string::npos);     // lines 30 ('X'), 36 ('Z')
    assert(cmdErr.find("invalid movie type") != std::string::npos);      // line 14 (B ... D Z ...)
    assert(cmdErr.find("only D media supported") != std::string::npos);  // line 47 (B 1000 Z F ...)
    assert(cmdErr.find("customer 1234 not found") != std::string::npos); // line 45
    assert(cmdErr.find("movie not found in inventory") != std::string::npos); // lines 46, 53
    assert(cmdErr.find("is out of stock") != std::string::npos);         // lines 50-52

    // ---- Stock: the "Harold and Maude" / Ruth Gordon classic starts with
    // 10 copies and is borrowed 13 times / returned 4 times over the script,
    // bottoming out at 0 — at which point lines 50, 51, and 52 are correctly
    // rejected as out-of-stock borrows. The final inventory ('I' on line 54)
    // must print this edition with stock 0.
    assert(out.find("C 0 Harold and Maude Hal Ashby Ruth Gordon 3 1971")
           != std::string::npos);

    // ---- Borrow histories: final transaction counts for the customers the
    // script queries with 'H', each tallied by hand from every successful
    // B/R line naming that customer.
    struct Expectation { int id; size_t historySize; };
    const Expectation expected[] = {
        {1000, 5},  // borrows: lines 6, 7, 17, 20, 22  (lines 46/47/53 rejected)
        {1111, 4},  // borrows: lines 13, 21, 28, 31    (line 14 rejected)
        {5000, 7},  // B/R pairs: lines 10-11, 15-16, 32, 35, 37
        {8000, 5},  // lines 5, 25, 27, 29, 38
        {8888, 5},  // lines 12, 23, 24, 33, 39
    };
    for (const auto& e : expected) {
        Customer* cust = db.customers.find(e.id);
        assert(cust != nullptr);
        assert(cust->getHistory().size() == e.historySize);
    }

    // ---- Customers never mentioned in any B/R line keep an empty history.
    Customer* untouched = db.customers.find(6666);
    assert(untouched != nullptr);
    assert(untouched->getHistory().empty());

    std::cout << "All end-to-end tests passed.\n";
    return 0;
}
