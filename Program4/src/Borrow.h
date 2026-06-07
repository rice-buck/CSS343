#pragma once
#include <string>
#include "Transaction.h"

class Borrow : public Transaction {
private:
    int customerID;
    std::string movieKey;

public:
    Borrow(Database& db, int cID, const std::string& mKey);

    void doTrans() override;
};
