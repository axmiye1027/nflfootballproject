// backendManager.h

#pragma once

#include "stadium.h"

#include <iostream>
#include <vector>

using namespace std;

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
    vector<Stadium> stadiums; // MAP OR UNORDERED MAP -- NOT FROM STL (vector for now bc idk)

    bool isAdmin;
};