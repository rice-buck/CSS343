#include "Drama.h"

Drama::Drama(char gen, int stk, const std::string& movieT, const std::string& direc, int yr) 
    : Movie(gen, stk, movieT, direc, yr) {}

void Drama::print(std::ostream& os) const{
    os << genre << " " << stock << " " << title << " " << director << " " << year;
}