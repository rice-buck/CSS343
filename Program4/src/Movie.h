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

private:
int movieID;

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

int getMovieID() const;
void setMovieID(int id);

//hashKey
std::string genHashKey();

void print(std::ostream& os) const;

friend std::ostream& operator<<(std::ostream& os, const Movie& movie);

};