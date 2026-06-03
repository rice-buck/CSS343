#include "HashTable.h"
#include "Movie.h"
#include "Customer.h"

class Database{

public:

HashTable<int, Movie> movies;
HashTable<int, Customer> customers;

Database();

int addMovie(Movie& newMovie);

void addCustomer(Customer newCust);

void addCustomerTransaction(int customerID, const std::string& t);

void borrowMovie(int ID, int movieKey);

void returnMovie(int ID, int movieKey);

void inventory();

void history(int ID);

private:
    int nextMovieID;
};