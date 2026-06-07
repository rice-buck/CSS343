// ============================================================================
// Operations.cpp
// ============================================================================
#include "Operations.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "Movie.h"
#include "Comedy.h"
#include "Drama.h"
#include "Classic.h"
#include "Customer.h"
#include "Database.h"

// ----------------------------------------------------------------------------
// Utility
// ----------------------------------------------------------------------------

std::string Operations::trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// ----------------------------------------------------------------------------
// readMovies — file format:
//   F, Stock, Director, Title, Year
//   D, Stock, Director, Title, Year
//   C, Stock, Director, Title, Actor Month Year
// ----------------------------------------------------------------------------

void Operations::readMovies(const std::string& filename, Database& db) {
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Operations: cannot open movie file '"
                  << filename << "'\n";
        return;
    }
    std::string line;
    while (std::getline(in, line)) {
        if (trim(line).empty()) continue;
        if (Movie* m = parseMovieRecord(line)) {
            db.addMovie(m);   // Database takes ownership
        }
    }
}

Movie* Operations::parseMovieRecord(const std::string& line) {
    std::istringstream iss(line);
    std::string field;

    // Five comma-separated fields. The last one's interpretation depends
    // on the movie code, so we read four standard ones and then the rest.
    auto nextField = [&]() -> bool {
        return static_cast<bool>(std::getline(iss, field, ','));
    };

    // Field 1: movie code (single character after trimming)
    if (!nextField()) return nullptr;
    std::string codeStr = trim(field);
    if (codeStr.empty()) return nullptr;
    char code = codeStr[0];

    // Field 2: stock
    if (!nextField()) return nullptr;
    int stock = 0;
    try { stock = std::stoi(trim(field)); }
    catch (...) {
        std::cerr << "Operations: bad stock value in: " << line << "\n";
        return nullptr;
    }

    // Field 3: director
    if (!nextField()) return nullptr;
    std::string director = trim(field);

    // Field 4: title
    if (!nextField()) return nullptr;
    std::string title = trim(field);

    // Field 5: year (Comedy/Drama) OR "Actor Month Year" (Classic)
    if (!std::getline(iss, field)) return nullptr;
    std::string tail = trim(field);

    switch (code) {
        case 'F': {                                  // Comedy
            try {
                int year = std::stoi(tail);
                return new Comedy(code, stock, title, director, year);
            } catch (...) {
                std::cerr << "Operations: bad year in comedy: " << line << "\n";
                return nullptr;
            }
        }
        case 'D': {                                  // Drama
            try {
                int year = std::stoi(tail);
                return new Drama(code, stock, title, director, year);
            } catch (...) {
                std::cerr << "Operations: bad year in drama: " << line << "\n";
                return nullptr;
            }
        }
        case 'C': {                                  // Classic
            // Pull tokens off the end: last is year, next-to-last is month,
            // everything before is the actor's name (one or more words).
            std::istringstream split(tail);
            std::vector<std::string> tokens;
            std::string tok;
            while (split >> tok) tokens.push_back(tok);
            if (tokens.size() < 3) {
                std::cerr << "Operations: bad classic format: " << line << "\n";
                return nullptr;
            }
            try {
                int year  = std::stoi(tokens.back()); tokens.pop_back();
                int month = std::stoi(tokens.back()); tokens.pop_back();
                std::string actor;
                for (size_t i = 0; i < tokens.size(); ++i) {
                    if (i) actor += ' ';
                    actor += tokens[i];
                }
                return new Classic(code, stock, title,director, actor, month, year);
            } catch (...) {
                std::cerr << "Operations: bad month/year in classic: "
                          << line << "\n";
                return nullptr;
            }
        }
        default:
            std::cerr << "Operations: invalid movie code '" << code
                      << "' in: " << line << "\n";
            return nullptr;
    }
}

// ----------------------------------------------------------------------------
// readCustomers — file format:
//   CustomerID LastName FirstName
// ----------------------------------------------------------------------------

void Operations::readCustomers(const std::string& filename, Database& db) {
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Operations: cannot open customer file '"
                  << filename << "'\n";
        return;
    }
    std::string line;
    while (std::getline(in, line)) {
        if (trim(line).empty()) continue;

        std::istringstream iss(line);
        int id;
        std::string lastName, firstName;
        if (!(iss >> id >> lastName >> firstName)) {
            std::cerr << "Operations: malformed customer record: "
                      << line << "\n";
            continue;
        }
        // Customer constructor is (id, firstName, lastName) here; swap
        // the order if your Customer.h has them the other way around.
        db.addCustomer(Customer(id, firstName, lastName));
    }
}

// ----------------------------------------------------------------------------
// readCommands — file format:
//   I                                       -> show inventory
//   H <id>                                  -> show history for customer
//   B <id> D <type> <movie-id-fields...>    -> borrow
//   R <id> D <type> <movie-id-fields...>    -> return
// ----------------------------------------------------------------------------

void Operations::readCommands(const std::string& filename, Database& db) {
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Operations: cannot open command file '"
                  << filename << "'\n";
        return;
    }
    std::string line;
    while (std::getline(in, line)) {
        if (trim(line).empty()) continue;

        std::istringstream iss(line);
        char action;
        if (!(iss >> action)) continue;

        switch (action) {
            case 'I':
                db.inventory();
                break;

            case 'H': {
                int id;
                if (!(iss >> id)) {
                    std::cerr << "Operations: H missing customer ID: "
                              << line << "\n";
                    break;
                }
                db.history(id);
                break;
            }

            case 'B':
            case 'R': {
                int id;
                char media, type;
                if (!(iss >> id >> media >> type)) {
                    std::cerr << "Operations: B/R missing fields: "
                              << line << "\n";
                    break;
                }
                if (media != 'D') {
                    std::cerr << "Operations: only D media supported, got '"
                              << media << "': " << line << "\n";
                    break;
                }
                auto stub = parseMovieIdentifier(type, iss);
                if (!stub) {
                    std::cerr << "Operations: cannot parse movie in: "
                              << line << "\n";
                    break;
                }
                const std::string key = stub->genHashKey();
                if (action == 'B') db.borrowMovie(id, key);
                else               db.returnMovie(id, key);
                break;
            }

            default:
                std::cerr << "Operations: invalid action code '"
                          << action << "' in: " << line << "\n";
                break;
        }
    }
}

// ----------------------------------------------------------------------------
// parseMovieIdentifier
//
// Command-line identifier formats per the spec:
//   F: "Title, Year"                   (e.g. Pirates of the Caribbean, 2003)
//   D: "Director, Title"               (assumed — spec gives no example)
//   C: "Month Year FirstName Last..."  (e.g. 9 1938 Katherine Hepburn)
//
// The returned Movie is a stub used only for hashKey() lookup; its stock and
// other non-identifying fields are placeholders.
// ----------------------------------------------------------------------------

std::unique_ptr<Movie>
Operations::parseMovieIdentifier(char type, std::istringstream& iss) {
    switch (type) {
        case 'F': {
            std::string rest;
            std::getline(iss, rest);
            rest = trim(rest);
            // Year is the last comma-separated field. Title may itself
            // contain spaces (and could contain commas — split on the LAST
            // comma so the year ends up correct).
            size_t lastComma = rest.find_last_of(',');
            if (lastComma == std::string::npos) return nullptr;
            std::string title   = trim(rest.substr(0, lastComma));
            std::string yearStr = trim(rest.substr(lastComma + 1));
            try {
                int year = std::stoi(yearStr);
                return std::make_unique<Comedy>(title, year);
            } catch (...) { return nullptr; }
        }

        case 'D': {
            std::string rest;
            std::getline(iss, rest);
            rest = trim(rest);
            size_t comma = rest.find(',');
            if (comma == std::string::npos) return nullptr;
            std::string director = trim(rest.substr(0, comma));
            std::string title    = trim(rest.substr(comma + 1));
            // The command format trails the title with a comma
            // ("Director, Title,") — strip it so the key matches the
            // stored title exactly.
            if (!title.empty() && title.back() == ',') title.pop_back();
            title = trim(title);
            return std::make_unique<Drama>(director, title);
        }

        case 'C': {
            int month, year;
            if (!(iss >> month >> year)) return nullptr;
            // Actor name is everything left in the stream (one or more words).
            std::string actor, tok;
            while (iss >> tok) {
                if (!actor.empty()) actor += ' ';
                actor += tok;
            }
            if (actor.empty()) return nullptr;
            return std::make_unique<Classic>(month, year, actor);
        }

        default:
            std::cerr << "Operations: invalid movie type '" << type << "'\n";
            return nullptr;
    }
}