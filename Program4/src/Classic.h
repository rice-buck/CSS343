#pragma once
#include "Movie.h"

class Classic : public Movie {
private:
    int month;
    std::string actor;

public:
    // Full constructor — used when loading the movie file.
    Classic(char gen, int stk, const std::string& movieT,
            const std::string& direc, const std::string& act,
            int mo, int yr);

    // Stub constructor — used only for command lookup (genHashKey only).
    Classic(int mo, int yr, const std::string& act);

    std::string getActor() const;
    int getMonth() const;

    std::string genHashKey() const override;
    void print(std::ostream& os) const override;
};
