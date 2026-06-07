#include "Database.h"
#include "Classic.h"

#include <algorithm>
#include <iostream>

Database::Database() {}

Database::~Database() {
    for (Movie* m : movieStore) delete m;
}

void Database::addMovie(Movie* newMovie) {
    const std::string key = newMovie->genHashKey();
    if (movies.contains(key)) {
        // Spec says each (title, actor) Classic edition is its own entry, and
        // Comedy/Drama identity is unique, so a duplicate key here means a
        // genuinely repeated record — keep the first, discard the rest.
        std::cerr << "Database: duplicate movie entry ignored: " << key << "\n";
        delete newMovie;
        return;
    }
    movieStore.push_back(newMovie);
    movies.insert(key, newMovie);
}

void Database::addCustomer(const Customer& newCust) {
    customers.insert(newCust.getID(), newCust);
}

std::string Database::transactionLabel(const Movie* m, const char* verb) {
    return std::string(verb) + ": " + m->getGenre() + " \"" + m->getMovieTitle()
           + "\" (" + std::to_string(m->getYear()) + ")";
}

void Database::borrowMovie(int custID, const std::string& movieKey) {
    Customer* cust = customers.find(custID);
    if (!cust) {
        std::cerr << "Error: customer " << custID << " not found — borrow discarded\n";
        return;
    }

    Movie** slot = movies.find(movieKey);
    if (!slot) {
        std::cerr << "Error: movie not found in inventory — borrow discarded\n";
        return;
    }
    Movie* movie = *slot;

    if (movie->getStock() <= 0) {
        std::cerr << "Error: \"" << movie->getMovieTitle()
                  << "\" is out of stock — borrow discarded\n";
        return;
    }

    movie->setStock(movie->getStock() - 1);
    cust->recordBorrow(movieKey);
    cust->addTransaction(transactionLabel(movie, "Borrowed"));
}

void Database::returnMovie(int custID, const std::string& movieKey) {
    Customer* cust = customers.find(custID);
    if (!cust) {
        std::cerr << "Error: customer " << custID << " not found — return discarded\n";
        return;
    }

    Movie** slot = movies.find(movieKey);
    if (!slot) {
        std::cerr << "Error: movie not found in inventory — return discarded\n";
        return;
    }
    Movie* movie = *slot;

    if (!cust->recordReturn(movieKey)) {
        std::cerr << "Error: customer " << custID << " never borrowed \""
                  << movie->getMovieTitle() << "\" — return discarded\n";
        return;
    }

    movie->setStock(movie->getStock() + 1);
    cust->addTransaction(transactionLabel(movie, "Returned"));
}

void Database::inventory() const {
    std::vector<Movie*> comedies, dramas, classics;
    for (Movie* m : movieStore) {
        switch (m->getGenre()) {
            case 'F': comedies.push_back(m); break;
            case 'D': dramas.push_back(m);   break;
            case 'C': classics.push_back(m); break;
            default: break;
        }
    }

    // Comedies: by Title, then Year
    std::sort(comedies.begin(), comedies.end(), [](const Movie* a, const Movie* b) {
        if (a->getMovieTitle() != b->getMovieTitle())
            return a->getMovieTitle() < b->getMovieTitle();
        return a->getYear() < b->getYear();
    });

    // Dramas: by Director, then Title
    std::sort(dramas.begin(), dramas.end(), [](const Movie* a, const Movie* b) {
        if (a->getDirector() != b->getDirector())
            return a->getDirector() < b->getDirector();
        return a->getMovieTitle() < b->getMovieTitle();
    });

    // Classics: by Release Date (year, then month), then Major Actor
    std::sort(classics.begin(), classics.end(), [](const Movie* a, const Movie* b) {
        const Classic* ca = static_cast<const Classic*>(a);
        const Classic* cb = static_cast<const Classic*>(b);
        if (ca->getYear() != cb->getYear())  return ca->getYear() < cb->getYear();
        if (ca->getMonth() != cb->getMonth()) return ca->getMonth() < cb->getMonth();
        return ca->getActor() < cb->getActor();
    });

    std::cout << "===== Inventory =====\n";

    std::cout << "\n-- Comedies --\n";
    for (const Movie* m : comedies) { m->print(std::cout); std::cout << "\n"; }

    std::cout << "\n-- Dramas --\n";
    for (const Movie* m : dramas) { m->print(std::cout); std::cout << "\n"; }

    std::cout << "\n-- Classics --\n";
    for (const Movie* m : classics) { m->print(std::cout); std::cout << "\n"; }

    std::cout << "=====================\n";
}

void Database::history(int custID) {
    Customer* cust = customers.find(custID);
    if (!cust) {
        std::cerr << "Error: customer " << custID << " not found — history discarded\n";
        return;
    }
    std::cout << "----- History for customer " << custID << " (newest first) -----\n";
    cust->printHistory();
    std::cout << "----------------------------------------------------\n";
}
