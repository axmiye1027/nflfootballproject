#include "../includes.h"
#include "stadium.h"

#include "hash.h"
#include "dfs.h"
#include "bfs.h"

#ifndef BACKENDMANAGER_H
#define BACKENDMANAGER_H

class BackendManager
{
public:
    BackendManager();
    ~BackendManager();

    bool login(string username, string password);

    
    void AddTeam();
    void AddSouvenir();
    void RemoveSouvinir();
    void ModifySouvinirPrice();
    // Move team and souvenir functions to stadium class?

    // Need functions for DB to stadiums data structure

private:
    // add stadium manager class here

    DoubleHashTable<Stadium> stadiums;

    AdjacencyList   dfsList;   // list used for dfs
    AdjacencyMatrix bfsMatrix; // matrix used for bfs

    bool isAdmin;
};

#endif