#include "Transaction.h"

class History : public Transaction {
private:
int customerID;

public:
History(Database& db, int cID);

void doTrans() override;

};