#include "Customer.h"
#include <iostream>
#include <stack>

//constructor 
Customer::Customer(int ID, std::string fName, std::string lName) : CustomerID(ID), firstName(fName), lastName(lName) {}

//Methods
int Customer::getID(){
    return CustomerID;
}

void Customer::setID(int newID){
    CustomerID = newID; 
}

std::string Customer::getFName(){
    return firstName;
}

std::string Customer::getLName(){
    return lastName; 
}

void Customer::addTransaction(std::string transac){
    customerHistory.push(transac);
}

std::stack<std::string> Customer::getHistory(){
    return customerHistory;
}

void Customer::printHistory(){
    std::stack<std::string> tempStack;
    tempStack = customerHistory;
    while(!tempStack.empty()){
        std::cout << tempStack.top() << std::endl;
        tempStack.pop();
    }
}