// ============================================================================
// Operations.h
// ----------------------------------------------------------------------------
// File-input layer for the movie rental system. Opens each of the three input
// files (movies, customers, commands), parses every line, and forwards the
// resulting objects/actions to Database. Operations does not own any movie or
// customer data — it just constructs them and hands them off.
// ============================================================================
#pragma once

#include <string>
#include <sstream>
#include <memory>

class Database;
class Movie;

class Operations {
public:
    // Read every movie line in `filename` and add valid movies to `db`.
    // Lines with an unknown movie code are reported on stderr and skipped.
    void readMovies(const std::string& filename, Database& db);

    // Read every customer line and add them to `db`.
    void readCustomers(const std::string& filename, Database& db);

    // Read every command line and dispatch on the action code (I/H/B/R).
    // Unknown actions, unknown movie types, and malformed lines are reported
    // on stderr and the line is discarded.
    void readCommands(const std::string& filename, Database& db);

private:
    // Parse one line from the movie file. Caller owns the returned pointer
    // (Database takes ownership when addMovie is called). Returns nullptr if
    // the line is malformed or the movie code is invalid.
    Movie* parseMovieRecord(const std::string& line);

    // Parse the movie-identifier portion of a B or R command. Returns a stub
    // Movie whose only purpose is to compute hashKey() for the lookup —
    // stock/director/etc. are set to placeholders. Returns nullptr on
    // malformed input or an invalid type code.
    std::unique_ptr<Movie> parseMovieIdentifier(char type,
    std::istringstream& rest);

    // Strip leading and trailing whitespace.
    static std::string trim(const std::string& s);
};