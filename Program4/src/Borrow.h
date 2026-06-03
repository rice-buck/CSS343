#include "Transaction.h"

class Borrow : public Transaction {
private:
int customerID;
int movieID;

public: 
Borrow(Database& db, int cID, int mID);

void doTrans() override;

};