#include "Movie.h"


//drama publicly inherits from Movie
class Drama : public Movie {

public:
//constructor
Drama(char gen, int stk, const std::string& movieT, const std::string& direc, int yr);
};