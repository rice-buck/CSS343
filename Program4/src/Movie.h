#include <string>
#pragma once

class Movie{

protected:

char genre;
int stock;
std::string title;
std::string director;
int year;


public:
//constructor
Movie(char gen, int stk, const std::string& movieT, const std::string& direc, int yr);

//getGenre
char getGenre();

//getStock
int getStock();

//setStock
bool setStock(int newStock);

//getMovieTitle
std::string getMovieTitle();

//getDirector
std::string getDirector();

//getYear
int getYear();

//hashKey
std::string genHashKey();

};