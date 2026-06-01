#include "HashTable.h"
#include "Movie.h"
#include "Customer.h"

class Database{
private: 
//HashTable<int, Movie> movies; 
HashTable<int, Customer> customers;

public: 
Database();

void addMovie(Movie newMovie);

void addCustomer(Customer newCustomer);

void borrowMovie(int ID, int movieKey);

void returnMovie(int ID, int movieKey);

void invetory();

void history(int ID);
};