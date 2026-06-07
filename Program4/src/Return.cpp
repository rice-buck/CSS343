#include "Return.h"

Return::Return(Database& db, int cID, const std::string& mKey)
    : Transaction(db), customerID(cID), movieKey(mKey) {}

void Return::doTrans() {
    dB.returnMovie(customerID, movieKey);
}
