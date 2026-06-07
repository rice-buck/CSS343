#pragma once
#include <string>
#include <ostream>

class Movie {
protected:
    char genre;
    int stock;
    std::string title;
    std::string director;
    int year;

public:
    Movie();
    Movie(char gen, int stk, const std::string& movieT,
          const std::string& direc, int yr);

    virtual ~Movie() = default;

    char getGenre() const;
    int getStock() const;
    void setStock(int newStock);
    std::string getMovieTitle() const;
    std::string getDirector() const;
    int getYear() const;

    // Content-based key used for hash-table lookup; each subtype overrides.
    virtual std::string genHashKey() const;

    virtual void print(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os, const Movie& movie);
};
