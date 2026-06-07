#include "Database.h"
#pragma once

class Transaction{
protected:
Database& dB;

public:
Transaction(Database& db) : dB(db) {}

virtual ~Transaction() = default;

virtual void doTrans() = 0;

Database& getDB();

};