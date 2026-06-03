#include "Movie.h"
#include <stdexcept>

//╰（‵□′）╯

//constructor
Movie::Movie(char gen, int stk, const std::string& movieT, const std::string& direc, int yr) 
    : genre(gen), stock(stk), title(movieT), director(direc), year(yr), movieID(0) {
    //error handling  idk if throwing an exception is the best way for error handling or not???
    //would have to implement try catch block where ever objects are instantiated (eg. main)...
    if(gen != 'F' && gen != 'D' && gen != 'C' ) {
        throw std::invalid_argument("Error: Genres only include 'F', 'D', and 'C'!");
    }
    if(stk < 0) throw std::invalid_argument("Error: Stock cannot be negative!");

    if(yr < 0) throw std::invalid_argument("Error: Year cannot be negative!");

    if(movieT == "") throw std::invalid_argument("Error: Title cannot be empty!");

    if(direc == "") throw std::invalid_argument("Error: Director cannot be empty!");
    }
//default constructor
Movie::Movie() : genre(' '), stock(0), title(""), director(""), year(0), movieID(0) {}
    

//getGenre
char Movie::getGenre(){
    return genre;
}

//getStock
int Movie::getStock(){
    return stock;
}

//setStock
void Movie::setStock(int newStock){
    if(newStock >= 0) stock = newStock;
}

//getMovieTitle
std::string Movie::getMovieTitle(){
    return title;
}

//getDirector
std::string Movie::getDirector(){
    return director;
}

//getYear
int Movie::getYear(){
    return year;
}

int Movie::getMovieID() const {
    return movieID;
}

void Movie::setMovieID(int id) {
    movieID = id;
}

//hashKey
//not sure how we should do this yet
std::string Movie::genHashKey(){
    return "boo-yah!";
}

void Movie::print(std::ostream& os) const{
    os << genre << " " << stock << " " << title << " " << director << " " << year;
}

std::ostream& operator<<(std::ostream& os, const Movie& movie){
     movie.print(os);
    return os;
}