#include "Customer.h"
#include <iostream>

Customer::Customer(int ID, const std::string& fName, const std::string& lName)
    : customerID(ID), firstName(fName), lastName(lName) {}

int  Customer::getID() const { return customerID; }
void Customer::setID(int newID) { customerID = newID; }

std::string Customer::getFName() const { return firstName; }
std::string Customer::getLName() const { return lastName; }

void Customer::addTransaction(const std::string& transac) {
    customerHistory.push(transac);
}

void Customer::recordBorrow(const std::string& movieKey) {
    ++borrowedCounts[movieKey];
}

bool Customer::recordReturn(const std::string& movieKey) {
    auto it = borrowedCounts.find(movieKey);
    if (it == borrowedCounts.end() || it->second <= 0) return false;
    if (--it->second == 0) borrowedCounts.erase(it);
    return true;
}

bool Customer::hasBorrowed(const std::string& movieKey) const {
    auto it = borrowedCounts.find(movieKey);
    return it != borrowedCounts.end() && it->second > 0;
}

std::stack<std::string> Customer::getHistory() const {
    return customerHistory;
}

void Customer::printHistory() const {
    std::stack<std::string> tempStack = customerHistory;
    while (!tempStack.empty()) {
        std::cout << tempStack.top() << std::endl;
        tempStack.pop();
    }
}

std::ostream& operator<<(std::ostream& os, const Customer& cust) {
    return os << cust.firstName << " " << cust.lastName << " " << cust.customerID;
}
