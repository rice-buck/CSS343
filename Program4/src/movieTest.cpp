#include "Comedy.h"
#include "Drama.h"
#include "Classic.h"
#include <iostream>

int main(){

    std::cout << "Testing Movie \n------------------------" << std::endl;

    Movie myMovie('F', 15, "Movie Movie frfr", "Donald Trump", 2007);

    std::cout << "Genre: " << myMovie.getGenre() << std::endl;
    std::cout << "Title: " << myMovie.getMovieTitle() << std::endl;
    std::cout << "Year: " << myMovie.getYear() << std::endl;
    std::cout << "\n------------------------\n" << std::endl;
    std::cout << "Testing Comedy \n------------------------" << std::endl;

    Comedy myComedy('F', 90, "Comedy Movie", "joe mama", 2067);

    std::cout << "Genre: " << myComedy.getGenre() << std::endl;

    std::cout << "Title: " << myComedy.getMovieTitle() << std::endl;
    std::cout << "Year: " << myComedy.getYear() << std::endl;
    std::cout << "\n------------------------\n" << std::endl;
    std::cout << "Testing Drama \n------------------------ "<< std::endl;

    Drama myDrama('D', 420, "Drama Movie", "Kamala Harris", 6767);

    std::cout << "Genre: " << myDrama.getGenre() << std::endl;
    std::cout << "Title: " << myDrama.getMovieTitle() << std::endl;
    std::cout << "Year: " << myDrama.getYear() << std::endl;
    std::cout << "\n------------------------\n" << std::endl;


    std::cout << "Testing Classic \n------------------------ "<< std::endl;

    Classic myClassic('C', 9, "Classic Movie", "Benjamin Netanyahu", "JoJo Siwa", 12, 2100);

    std::cout << "Genre: " << myClassic.getGenre() << std::endl;
    std::cout << "Title: " << myClassic.getMovieTitle() << std::endl;
    std::cout << "Month: " << myClassic.getMonth() << std::endl;
    std::cout << "Year: " << myClassic.getYear() << std::endl;
    std::cout << "Director: " << myClassic.getDirector() << std::endl;
    std::cout << "Actor: " << myClassic.getActor() << std::endl;
    std::cout << "\n------------------------\n" << std::endl;

    std::cout << "====== End Test ======" << std::endl;
}