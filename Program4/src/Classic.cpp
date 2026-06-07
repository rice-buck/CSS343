#include "Classic.h"
#include <stdexcept>
#include <string>

Classic::Classic(char gen, int stk, const std::string& movieT,
                 const std::string& direc, const std::string& act,
                 int mo, int yr)
    : Movie(gen, stk, movieT, direc, yr), month(mo), actor(act) {
    if (act.empty())
        throw std::invalid_argument("Error: actor cannot be empty");
    if (mo < 1 || mo > 12)
        throw std::invalid_argument("Error: month out of range");
}

// Stub: only month + year + actor needed to reproduce the hash key.
// Uses placeholder title/director so Movie's validation passes.
Classic::Classic(int mo, int yr, const std::string& act)
    : Movie('C', 0, "stub", "stub", yr), month(mo), actor(act) {}

std::string Classic::getActor() const { return actor; }
int         Classic::getMonth() const { return month; }

std::string Classic::genHashKey() const {
    return "C:" + std::to_string(month) + ":" + std::to_string(year)
           + ":" + actor;
}

void Classic::print(std::ostream& os) const {
    os << genre << " " << stock << " " << title << " "
       << director << " " << actor << " " << month << " " << year;
}
