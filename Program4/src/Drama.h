#pragma once
#include "Movie.h"

class Drama : public Movie {
public:
    // Full constructor — used when loading the movie file.
    Drama(char gen, int stk, const std::string& movieT,
          const std::string& direc, int yr);

    // Stub constructor — used only for command lookup (genHashKey only).
    Drama(const std::string& direc, const std::string& movieT);

    std::string genHashKey() const override;
    void print(std::ostream& os) const override;
};
