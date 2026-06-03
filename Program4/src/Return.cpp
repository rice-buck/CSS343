#include "Return.h"

Return::Return(Database& db, int cID, int mID) : Transaction(db), customerID(cID), movieID(mID) {}

void Return::doTrans() {
    dB.returnMovie(customerID, movieID);
}