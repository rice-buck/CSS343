#include "Movie.h"
#include <stdexcept>

//╰（‵□′）╯

//constructor
Movie::Movie(char gen, int stk, const std::string& movieT, const std::string& direc, int yr) 
    : genre(gen), stock(stk), title(movieT), director(direc), year(yr) {
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
    

//getGenre
char Movie::getGenre(){
    return genre;
}

//getStock
int Movie::getStock(){
    return stock;
}

//setStock
bool Movie::setStock(int newStock){
    if(stock < 0) return false;

    return stock = newStock;
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

//hashKey
//not sure how we should do this yet
std::string Movie::genHashKey(){
    return "boo-yah!";
}

//compare()
//not sure what this is for again... 


