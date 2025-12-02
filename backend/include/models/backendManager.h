
#include <algorithm>

#include "../includes.h"
#include "stadium.h"

#include "hash.h"
#include "adjacencyList.h"
#include "adjacencyMatrix.h"
#include "database/databaseManager.h"


#ifndef BACKENDMANAGER_H
#define BACKENDMANAGER_H

// Define comparator structs
struct CompareByCapacity 
{
    bool operator()(const Stadium& a, const Stadium& b) const 
    {
        return a.getCapacity() > b.getCapacity();
    }
};

struct CompareByYearOpened 
{
    bool operator()(const Stadium& a, const Stadium& b) const 
    {
        return a.getYearOpened() > b.getYearOpened();
    }
};

struct CompareByStadiumName 
{
    bool operator()(const Stadium& a, const Stadium& b) const 
    {
        return a.getStadiumName() > b.getStadiumName();
    }
};

struct CompareByTeamName 
{
    bool operator()(const Stadium& a, const Stadium& b) const 
    {
        return a.getTeamName() > b.getTeamName();
    }
};


class BackendManager
{
public:
    BackendManager();
    ~BackendManager();

    bool login(string username, string password);

    void populateStadiums(); // Grabs Stadiums info from database and stores in stadiums
    void populateDistances(); // Grabs Distance info from database and stores in adjacencyMatrix and adjacencyList

    void addTeam();
    void addStadium(string teamName, string stadiumName, int capacity, string location, RoofType roofType, string surface,
            int yearOpened, string conference, string division, vector<Souvenir> souvenirList);
    bool updateStadium(int stadiumId, string teamName, string stadiumName, int capacity, string location, string roofType, string surface,
            int yearOpened, string conference, string division);
    void addSouvenir(int stadiumId, const string& souvenirName, double souvenirPrice);
    void removeSouvenir(int stadiumId,const string&  name);
    void modifySouvenirName(int stadiumId,const string&  oldName,const string& newName);
    void modifySouvenirPrice(int stadiumId, const string& name, double souvenirPrice);

    // Move team and souvenir functions to stadium class?

    // Need functions for DB to stadiums data structure

    vector<Stadium> getStadiumsAsVector();

    // Refer to the top of backendManager.cpp for what the Compare data type is used for
    template<typename Compare>
    vector<Stadium> sortStadiums(const vector<Stadium>&              stadiumsVect, Compare compare);
    vector<Stadium> sortStadiumsByStadiumName(const vector<Stadium>& stadiumsVect);
    vector<Stadium> sortStadiumsByTeam(const vector<Stadium>&        stadiumsVect, string teamName);
    vector<Stadium> sortStadiumsByDateOpened(const vector<Stadium>&  stadiumsVect);
    vector<Stadium> sortStadiumsByCapacity(const vector<Stadium>&    stadiumsVect);

    Stadium         getStadiumById(const vector<Stadium>&          stadiumsVect,int    id);
    Stadium         getStadiumByName(const vector<Stadium>&        stadiumsVect,string stadiumName);
    vector<Stadium> getStadiumsByRoofType(const vector<Stadium>&   stadiumsVect,string roofType);
    vector<Stadium> getStadiumsByConference(const vector<Stadium>& stadiumsVect,string conference);
    vector<Stadium> getStadiumsByDivision(const vector<Stadium>&   stadiumsVect,string division);
    vector<Stadium> getStadiumsByGrass(const vector<Stadium>&      stadiumsVect,string grassType);

    DoubleHashTable<Souvenir> getTeamSouvenirs(string teamName);

    int totalStadiumCapacity(const vector<Stadium>& stadiumsVect);

    vector<Stadium> filterStadiums(const vector<Stadium>& stadiumsVect, string search);

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