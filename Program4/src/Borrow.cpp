#include "Borrow.h"

Borrow::Borrow(Database& db, int cID, const std::string& mKey)
    : Transaction(db), customerID(cID), movieKey(mKey) {}

void Borrow::doTrans() {
    dB.borrowMovie(customerID, movieKey);
}
