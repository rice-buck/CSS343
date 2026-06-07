// ============================================================================
// main.cpp — movie rental inventory system entry point.
//
// Wires Operations to a Database and runs the three pipeline stages the spec
// calls for: load movies, load customers, then process commands. Defaults to
// the bundled test-cases data set; pass three filenames to override.
// ============================================================================
#include <iostream>
#include <string>

#include "Database.h"
#include "Operations.h"

int main(int argc, char* argv[]) {
    std::string moviesFile    = "test-cases/data4movies.txt";
    std::string customersFile = "test-cases/data4customers.txt";
    std::string commandsFile  = "test-cases/data4commands.txt";

    if (argc == 4) {
        moviesFile    = argv[1];
        customersFile = argv[2];
        commandsFile  = argv[3];
    } else if (argc != 1) {
        std::cerr << "Usage: " << argv[0]
                  << " [movies.txt customers.txt commands.txt]\n";
        return 1;
    }

    Database db;
    Operations ops;

    ops.readMovies(moviesFile, db);
    ops.readCustomers(customersFile, db);
    ops.readCommands(commandsFile, db);

    return 0;
}
