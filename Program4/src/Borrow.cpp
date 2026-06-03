#include "Borrow.h"

Borrow::Borrow(Database& db, int cID, int mID) : Transaction(db), customerID(cID), movieID(mID) {}

void Borrow::doTrans(){
    dB.borrowMovie(customerID, movieID);
}