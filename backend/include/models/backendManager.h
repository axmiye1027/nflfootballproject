
#include <algorithm>

#include "../includes.h"
#include "stadium.h"

#include "hash.h"
#include "dfs.h"
#include "bfs.h"
#include "database/databaseManager.h"


#ifndef BACKENDMANAGER_H
#define BACKENDMANAGER_H

class BackendManager
{
public:
    BackendManager();
    ~BackendManager();

    bool login(string username, string password);
    void printStadiums();

    // NEEDS TO BE UPDATED WITH SOUVENIRS
    void populateStadiums(); // Grabs Stadiums info from database and stores in stadiums

    void addTeam();
    void addSouvenir();
    void removeSouvinir();
    void modifySouvenirPrice();
    // Move team and souvenir functions to stadium class?

    // Need functions for DB to stadiums data structure

private:
    DoubleHashTable<Stadium> stadiums;

    AdjacencyList   dfsList;   // list used for dfs
    AdjacencyMatrix bfsMatrix; // matrix used for bfs

    DatabaseManager databaseManager;

    bool isAdmin;
};

#endif