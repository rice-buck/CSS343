#include "Transaction.h"

class Return : public Transaction {
private:
int customerID;
int movieID;

public: 
Return(Database& db, int cID, int mID);

void doTrans() override;

};