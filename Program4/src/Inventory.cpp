#include "Inventory.h"

Inventory::Inventory(Database& db) : Transaction(db) {}

void Inventory::doTrans(){
    dB.inventory();
}