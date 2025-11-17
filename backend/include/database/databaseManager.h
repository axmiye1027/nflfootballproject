// databaseManager.h

#include "sqlite3.h"
#include "includes.h"

class DatabaseManager
{
public:
    DatabaseManager(const string PATH = "backend/assets/database.db");
    ~DatabaseManager();


private:
    sqlite3* database;
};