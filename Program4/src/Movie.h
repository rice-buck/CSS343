#include <string>
#include <ostream>
#pragma once

class Movie{

protected:

char genre;
int stock;
std::string title;
std::string director;
int year;


public:

//default constructor
Movie();

//constructor
Movie(char gen, int stk, const std::string& movieT, const std::string& direc, int yr);

//getGenre
char getGenre();

//getStock
int getStock();

//setStock
void setStock(int newStock);

//getMovieTitle
std::string getMovieTitle();

//getDirector
std::string getDirector();

//getYear
int getYear();

//hashKey
std::string genHashKey();

uint64_t generateUniqueId();

void print(std::ostream& os) const;

friend std::ostream& operator<<(std::ostream& os, const Movie& movie);

};