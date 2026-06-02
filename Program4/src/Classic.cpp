#include "Classic.h"
#include <stdexcept>

Classic::Classic(char gen, int stk, const std::string& movieT, const std::string& direc, const std::string& act, int mo, int yr)
    : Movie(gen, stk, movieT, direc, yr), actor(act), month(mo) {
        if(act == "") throw std::invalid_argument("Error: Actor cannot be empty!");

        if(mo <= 0 || mo > 12) throw std::invalid_argument("Error: Month is out of range!");
    }

//getActor
std::string Classic::getActor(){
    return actor;
}

//getMonth
int Classic::getMonth(){
    return month;
}

void Classic::print(std::ostream& os) const{
    os << genre << " " << stock << " " << title << " " << director << " " << actor << " " << month << " " << year;
}