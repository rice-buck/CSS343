#include "Comedy.h"
#include <string>

Comedy::Comedy(char gen, int stk, const std::string& movieT,
               const std::string& direc, int yr)
    : Movie(gen, stk, movieT, direc, yr) {}

// Stub: only title + year needed to reproduce the hash key for lookup.
Comedy::Comedy(const std::string& movieT, int yr)
    : Movie('F', 0, movieT, "stub", yr) {}

std::string Comedy::genHashKey() const {
    return "F:" + title + ":" + std::to_string(year);
}

void Comedy::print(std::ostream& os) const {
    os << genre << " " << stock << " " << title
       << " " << director << " " << year;
}
