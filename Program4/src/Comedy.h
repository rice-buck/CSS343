#pragma once
#include "Movie.h"

class Comedy : public Movie {
public:
    // Full constructor — used when loading the movie file.
    Comedy(char gen, int stk, const std::string& movieT,
           const std::string& direc, int yr);

    // Stub constructor — used only for command lookup (genHashKey only).
    Comedy(const std::string& movieT, int yr);

    std::string genHashKey() const override;
    void print(std::ostream& os) const override;
};
