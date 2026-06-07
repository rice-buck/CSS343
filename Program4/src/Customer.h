#pragma once
#include <map>
#include <ostream>
#include <stack>
#include <string>

class Customer {
private:
    int customerID;
    std::string firstName;
    std::string lastName;
    std::stack<std::string> customerHistory;

    // Outstanding-borrow counts, keyed by movie content key. A map (rather
    // than a set) is required because the same title can be borrowed more
    // than once before any of those copies are returned — a set could not
    // tell "borrowed twice, returned once" from "borrowed once, returned
    // once" and would wrongly reject a legitimate second return.
    std::map<std::string, int> borrowedCounts;

public:
    Customer(int ID, const std::string& fName, const std::string& lName);

    int getID() const;
    void setID(int newID);
    std::string getFName() const;
    std::string getLName() const;

    void addTransaction(const std::string& transac);

    // Called when a borrow succeeds — records the movie key as outstanding.
    void recordBorrow(const std::string& movieKey);

    // Called when a return succeeds — removes the outstanding borrow.
    // Returns false if the customer never borrowed this movie.
    bool recordReturn(const std::string& movieKey);

    bool hasBorrowed(const std::string& movieKey) const;

    std::stack<std::string> getHistory() const;
    void printHistory() const;

    friend std::ostream& operator<<(std::ostream& os, const Customer& cust);
};
