#include "Drama.h"
#include <string>

Drama::Drama(char gen, int stk, const std::string& movieT,
             const std::string& direc, int yr)
    : Movie(gen, stk, movieT, direc, yr) {}

// Stub: only director + title needed to reproduce the hash key for lookup.
Drama::Drama(const std::string& direc, const std::string& movieT)
    : Movie('D', 0, movieT, direc, 0) {}

std::string Drama::genHashKey() const {
    return "D:" + director + ":" + title;
}

void Drama::print(std::ostream& os) const {
    os << genre << " " << stock << " " << title
       << " " << director << " " << year;
}
