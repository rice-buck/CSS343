#include "Database.h"
#include <ostream>
#include <string>

Database::Database() {}


void Database::addMovie(Movie newMovie){
    movies.insert(newMovie.generateUniqueId(), newMovie);
}

void Database::addCustomer(Customer newCust){
    customers.insert(newCust.getID(), newCust);
}


//LATER: make sure people borrow only if the book is greater than 0

void Database::borrowMovie(int ID, int movieKey){
    if(movies.contains(movieKey)){
        auto movieIt = movies.find(movieKey);
        movieIt->setStock(movieIt->getStock() - 1);
        //make trans string for adding to history
        std::string transaction;
        transaction = std::string("Borrowed: ") + movieIt->getGenre() + " " + movieIt->getMovieTitle() + " " + movieIt->getDirector() + " " + std::to_string(movieIt->getYear());
        customers.find(ID)->addTransaction(transaction);
    }
    
}

//LATER: make sure people cant add movies they dont own

void Database::returnMovie(int ID, int movieKey){
    if(movies.contains(movieKey)){
        auto movieIt = movies.find(movieKey);
        movieIt->setStock(movieIt->getStock() + 1);
        //make trans string for adding to history
        std::string transaction;
        transaction = std::string("Returned: ") + movieIt->getGenre() + " " + movieIt->getMovieTitle() + " " + movieIt->getDirector() + " " + std::to_string(movieIt->getYear());
        customers.find(ID)->addTransaction(transaction);
    }
}

void Database::invetory(){
    movies.print();
}

void Database::history(int ID){
    Customer* custIt = customers.find(ID);
    if (custIt != nullptr) {
        std::cout << "Newest\n";
        custIt->printHistory();
        std::cout << "Oldest\n";
    } else {
        std::cout << "Customer not found.\n";
    }
}
