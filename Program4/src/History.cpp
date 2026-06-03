#include "History.h"

History::History(Database& db, int cID) : Transaction(db), customerID(cID) {}

void History::doTrans(){
    dB.history(customerID);
}