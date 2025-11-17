// databaseManager.cpp

#include "database/databaseManager.h"

DatabaseManager::DatabaseManager(const string PATH)
{
    int opened = sqlite3_open("backend/assets/database.db", &database);

    if (opened != SQLITE_OK)
    {
        cerr << "not openeed";
    }
    else
    {
        cerr << "Opened Database!";
    }
}


DatabaseManager::~DatabaseManager()
{
    sqlite3_close(database);
}

