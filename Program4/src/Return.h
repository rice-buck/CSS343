#pragma once
#include <string>
#include "Transaction.h"

class Return : public Transaction {
private:
    int customerID;
    std::string movieKey;

public:
    Return(Database& db, int cID, const std::string& mKey);

    void doTrans() override;
};
