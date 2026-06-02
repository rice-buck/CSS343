#include "Movie.h"


//classic publicly inherits from Movie
class Classic : public Movie {
private:
int month;
std::string actor; 

public:
//constructor
Classic(char gen, int stk, const std::string& movieT, const std::string& direc, const std::string& act, int mo, int yr);

//getActor
std::string getActor();

//getMonth
int getMonth();

void print(std::ostream& os) const;
};