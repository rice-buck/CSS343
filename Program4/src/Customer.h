#include <stack>
#include <string>
#pragma once

class Customer{
private:
int CustomerID;
std::string firstName, lastName;
std::stack<std::string> customerHistory;

public:
//constructor 
Customer(int ID, std::string fName, std::string lName);

//Methods
int getID();

void setID(int newID);

std::string getFName();

std::string getLName();

void addTransaction(std::string transac);

std::stack<std::string>getHistory();

void printHistory();

friend std::ostream& operator<<(std::ostream& os, const Customer& cust);
};
