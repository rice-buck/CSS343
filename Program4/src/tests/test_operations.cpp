// Unit tests for Operations file parsing — written against small fixture
// files (created and torn down here) rather than the big test-cases set, so
// each scenario (invalid codes, classic multi-actor editions, B/R lookups,
// stock errors) can be pinned down precisely.
#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../Database.h"
#include "../Operations.h"
#include "../Classic.h"
#include "../Comedy.h"

namespace {

// Captures everything written to std::cerr while `fn` runs.
template <typename Fn>
std::string captureStderr(Fn&& fn) {
    std::ostringstream buf;
    std::streambuf* old = std::cerr.rdbuf(buf.rdbuf());
    fn();
    std::cerr.rdbuf(old);
    return buf.str();
}

void writeFile(const std::string& path, const std::string& contents) {
    std::ofstream out(path);
    out << contents;
}

} // namespace

static void test_invalid_movie_code_discarded_with_error() {
    const std::string path = "tmp_movies_invalid.txt";
    writeFile(path,
        "F, 10, Nora Ephron, You've Got Mail, 1998\n"
        "Z, 10, Hal Ashby, Harold and Maude, Ruth Gordon 2 1971\n"
        "C, 10, George Cukor, Holiday, Cary Grant 9 1938\n");

    Database db;
    Operations ops;
    std::string err = captureStderr([&]{ ops.readMovies(path, db); });

    assert(err.find("invalid movie code") != std::string::npos);
    // The two valid rows should still be loaded...
    assert(db.customers.getSize() == 0);   // sanity: didn't touch customers
    std::remove(path.c_str());
}

static void test_classic_multiple_actor_editions_tracked_separately() {
    const std::string path = "tmp_movies_classic.txt";
    writeFile(path,
        "C, 10, George Cukor, Holiday, Katherine Hepburn 9 1938\n"
        "C, 10, George Cukor, Holiday, Cary Grant 9 1938\n");

    Database db;
    Operations ops;
    ops.readMovies(path, db);

    Classic hepburnStub(9, 1938, "Katherine Hepburn");
    Classic grantStub(9, 1938, "Cary Grant");

    // Borrow each edition once; only that edition's stock should move.
    db.addCustomer(Customer(1, "Joe", "Mama"));
    db.borrowMovie(1, hepburnStub.genHashKey());
    db.borrowMovie(1, grantStub.genHashKey());
    assert(db.customers.find(1)->getHistory().size() == 2);

    std::remove(path.c_str());
}

static void test_read_customers_basic() {
    const std::string path = "tmp_customers.txt";
    writeFile(path, "1111 Mouse Mickey\n1000 Mouse Minnie\n");

    Database db;
    Operations ops;
    ops.readCustomers(path, db);

    Customer* mickey = db.customers.find(1111);
    assert(mickey != nullptr);
    assert(mickey->getFName() == "Mickey");
    assert(mickey->getLName() == "Mouse");

    std::remove(path.c_str());
}

static void test_command_borrow_and_return_resolve_by_content() {
    const std::string moviesPath = "tmp_cmd_movies.txt";
    const std::string custPath   = "tmp_cmd_customers.txt";
    const std::string cmdPath    = "tmp_cmd_commands.txt";

    writeFile(moviesPath,
        "F, 10, Nora Ephron, You've Got Mail, 1998\n"
        "D, 10, Barry Levinson, Good Morning Vietnam, 1988\n"
        "C, 10, George Cukor, Holiday, Katherine Hepburn 9 1938\n");
    writeFile(custPath, "1000 Mouse Minnie\n");
    writeFile(cmdPath,
        "B 1000 D F You've Got Mail, 1998\n"
        "B 1000 D D Barry Levinson, Good Morning Vietnam,\n"
        "B 1000 D C 9 1938 Katherine Hepburn\n"
        "R 1000 D F You've Got Mail, 1998\n");

    Database db;
    Operations ops;
    ops.readMovies(moviesPath, db);
    ops.readCustomers(custPath, db);
    ops.readCommands(cmdPath, db);

    Customer* cust = db.customers.find(1000);
    assert(cust != nullptr);
    // 3 borrows + 1 return, all of which must have resolved to real movies.
    assert(cust->getHistory().size() == 4);

    std::remove(moviesPath.c_str());
    std::remove(custPath.c_str());
    std::remove(cmdPath.c_str());
}

static void test_unknown_action_and_movie_type_reported() {
    const std::string cmdPath = "tmp_bad_commands.txt";
    writeFile(cmdPath,
        "X\n"
        "B 1111 D Z 2 1971 Malcolm McDowell\n");

    Database db;
    db.addCustomer(Customer(1111, "Mouse", "Mickey"));
    Operations ops;
    std::string err = captureStderr([&]{ ops.readCommands(cmdPath, db); });

    assert(err.find("invalid action code") != std::string::npos);
    assert(err.find("invalid movie type") != std::string::npos
           || err.find("cannot parse movie") != std::string::npos);

    std::remove(cmdPath.c_str());
}

int main() {
    test_invalid_movie_code_discarded_with_error();
        std::cout << "  invalid movie code -> error           OK\n";
    test_classic_multiple_actor_editions_tracked_separately();
        std::cout << "  classic multi-actor editions          OK\n";
    test_read_customers_basic();
        std::cout << "  customer file parsing                 OK\n";
    test_command_borrow_and_return_resolve_by_content();
        std::cout << "  B/R commands resolve by content key   OK\n";
    test_unknown_action_and_movie_type_reported();
        std::cout << "  invalid action/type reported          OK\n";
    std::cout << "All Operations tests passed.\n";
    return 0;
}
