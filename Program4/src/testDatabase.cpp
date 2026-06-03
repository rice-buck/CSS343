#include "Database.h"
#include "Customer.h"
#include "Comedy.h"
#include "Drama.h"
#include "Classic.h"
#include <iostream>

int main(){

    Database dB;

    Customer testCust(111, "Joe", "Mama");

    Movie testMovie('C', 23, "testMovie", "JoJo Siwa", 2027);
    Comedy testComedy('F', 40, "comedy1", "weee", 1089);
    Drama testDrama('D', 12, "testDrama", " Drama actor ", 2009);
    Classic testClassic('C', 45, "testClassic", "classic director", "classic actor", 6, 2008);


    dB.addCustomer(testCust);

    dB.addMovie(testMovie);
    dB.addMovie(testDrama);
    dB.addMovie(testComedy);
    dB.addMovie(testClassic);
    dB.invetory();
    dB.borrowMovie(111, 2);
    dB.returnMovie(111, 3);

    std::cout << "\nTesting history" << std::endl;
    std::cout << "---------------\n";
    dB.history(111);
      std::cout << "---------------\n";
    std::cout << "end history\n" << std::endl;

    std::cout << "\n";
    dB.invetory();

    std::cout << "\n";
    dB.customers.print();
   
}