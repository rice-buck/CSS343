#include "HashTable.h"
#include "Movie.h"
#include "Customer.h"

class Database{

public: 

HashTable<int, Movie> movies; 
HashTable<int, Customer> customers;

Database();

void addMovie(Movie newMovie);

void addCustomer(Customer newCust);

void addCustomerTransaction(int customerID, const std::string& t);

void borrowMovie(int ID, int movieKey);

void returnMovie(int ID, int movieKey);

void invetory();

void history(int ID);


};