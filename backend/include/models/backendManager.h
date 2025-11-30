
#include <algorithm>

#include "../includes.h"
#include "stadium.h"

#include "hash.h"
#include "adjacencyList.h"
#include "adjacencyMatrix.h"
#include "database/databaseManager.h"


#ifndef BACKENDMANAGER_H
#define BACKENDMANAGER_H

class BackendManager
{
public:
    BackendManager();
    ~BackendManager();

    bool login(string username, string password);

    void populateStadiums(); // Grabs Stadiums info from database and stores in stadiums
    void populateDistances(); // Grabs Distance info from database and stores in adjacencyMatrix and adjacencyList

    void addTeam();
    void addSouvenir(int stadiumId, const string& souvenirName, double souvenirPrice);
    void removeSouvenir(int stadiumId,const string&  name);
    void modifySouvenirName(int stadiumId,const string&  oldName,const string& newName);
    void modifySouvenirPrice(int stadiumId, const string& name, double souvenirPrice);
    void addStadium(string teamName, string stadiumName, int capacity, string location, RoofType roofType, string surface,
    int dateOpened, string conference, string division, vector<Souvenir> souvenirList);
    // Move team and souvenir functions to stadium class?

    // Need functions for DB to stadiums data structure

    vector<Stadium> getStadiumsAsVector();

    // Refer to the top of backendManager.cpp for what the Compare data type is used for
    vector<Stadium> sortStadiums(const vector<Stadium>&             stadiumsVect, Compare compare);
    vector<Stadium> sortStadiumsByStadium(const vector<Stadium>&    stadiumsVect);
    vector<Stadium> sortStadiumsByTeam(const vector<Stadium>&       stadiumsVect, string teamName);
    vector<Stadium> sortStadiumsByDateOpened(const vector<Stadium>& stadiumsVect);
    vector<Stadium> sortStadiumsByCapacity(const vector<Stadium>&   stadiumsVect);

    Stadium         getStadiumByName(const vector<Stadium>&        stadiumsVect,string stadiumName);
    vector<Stadium> getStadiumsByRoofType(const vector<Stadium>&   stadiumsVect,string roofType);
    vector<Stadium> getStadiumsByDivision(const vector<Stadium>&   stadiumsVect,string division);
    vector<Stadium> getStadiumsByConference(const vector<Stadium>& stadiumsVect,string conference);

    DoubleHashTable<Souvenir> getTeamSouvenirs(string teamName);

    int totalStadiumCapacity(const vector<Stadium>& stadiumsVect);

    vector<Stadium> filterStadiums(vector<Stadium> stadiumsVect, string search);

    /* ----- PRINT TO TERMINAL -----*/
    void printStadiums() const;

private:
    DoubleHashTable<Stadium> stadiums;

    AdjacencyList   adjacencyList;   // list used for dfs
    AdjacencyMatrix adjacencyMatrix; // matrix used for bfs, djikstra, mst

    DatabaseManager databaseManager;

    bool isAdmin;
};

#endif