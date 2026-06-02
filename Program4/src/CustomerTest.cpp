//TEST CUSTOMER CLASS 
#include "Customer.h"
#include <iostream>

int main() {
    Customer customer1(1, "Joe", "Mama");
    std::cout << "Customer ID: " << customer1.getID() << std::endl;
    std::cout << "First Name: " << customer1.getFName() << std::endl;
    std::cout << "Last Name: " << customer1.getLName() << std::endl;

    customer1.addTransaction("Purchased item A");
    customer1.addTransaction("Purchased item B");

    std::cout << "Customer History:" << std::endl;
    customer1.printHistory();

    std::cout << customer1 << std::endl;

    return 0;
}