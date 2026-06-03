#include "Transaction.h"

class Inventory : public Transaction {
public:
Inventory(Database& db);

void doTrans() override;

};