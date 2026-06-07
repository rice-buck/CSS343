#include "Movie.h"
#include <stdexcept>

Movie::Movie()
    : genre(' '), stock(0), title(""), director(""), year(0) {}

Movie::Movie(char gen, int stk, const std::string& movieT,
             const std::string& direc, int yr)
    : genre(gen), stock(stk), title(movieT), director(direc), year(yr) {
    if (gen != 'F' && gen != 'D' && gen != 'C')
        throw std::invalid_argument("Error: genre must be F, D, or C");
    if (stk < 0)
        throw std::invalid_argument("Error: stock cannot be negative");
    if (yr < 0)
        throw std::invalid_argument("Error: year cannot be negative");
    if (movieT.empty())
        throw std::invalid_argument("Error: title cannot be empty");
    if (direc.empty())
        throw std::invalid_argument("Error: director cannot be empty");
}

char Movie::getGenre() const { return genre; }
int  Movie::getStock() const { return stock; }

void Movie::setStock(int newStock) {
    if (newStock >= 0) stock = newStock;
}

std::string Movie::getMovieTitle() const { return title; }
std::string Movie::getDirector()   const { return director; }
int         Movie::getYear()       const { return year; }

std::string Movie::genHashKey() const { return ""; }

void Movie::print(std::ostream& os) const {
    os << genre << " " << stock << " " << title
       << " " << director << " " << year;
}

std::ostream& operator<<(std::ostream& os, const Movie& movie) {
    movie.print(os);
    return os;
}
