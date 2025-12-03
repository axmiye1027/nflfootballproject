/**
 * @file backendManager.h
 * @brief functions for adding and modifying or using database data
 * 
 * Does a variety of features such as comparing, editing data, adding data,
 * removing data and calculations 
 */
#include <algorithm>

#include "../includes.h"
#include "stadium.h"

#include "hash.h"
#include "adjacencyList.h"
#include "adjacencyMatrix.h"
#include "database/databaseManager.h"


#ifndef BACKENDMANAGER_H
#define BACKENDMANAGER_H

/**
 * Define comparator structs
 */
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

// struct PathReturn 
// {
//     vector<string> path;
//     int distanceTraveled;

//     PathReturn(vector<string> path, int distanceTraveled) : path(path), distanceTraveled(distanceTraveled) {}
// };

class BackendManager
{
public:
    BackendManager();
    ~BackendManager();

    /**
     * @param username user input for username
     * @param password user input for password
     * @return boolean, true or false
     * 
     * User inputs 2 strings, a username and password and checks if they're valid
     * credentials. If valid returns true, invalid returns false
     */
    bool login(string username, string password);

    /**
     * @brief reads data from file and adds to database
     * @param json file
     * 
     * Reads data from a file and adds the team(s) and its related data to the 
     * database. Automatically assumes the team will have the default souveniers
     * Will get distances and other info related to stadium
     */
    void importStadiums(const string& json);

    /**
     * Grabs Stadiums info from database and stores in stadiums
     */
    void populateStadiums();
    /**
     * Grabs Distance info from database and stores in adjacencyMatrix and adjacencyList
     */
    void populateDistances();

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
    vector<Stadium> getStadiumsBySurface(const vector<Stadium>&    stadiumsVect,string surface);

    DoubleHashTable<Souvenir> getTeamSouvenirs(string teamName);

    int totalStadiumCapacity(const vector<Stadium>& stadiumsVect);

    int calculateBFS(const string& startingStadium);
    int calculateMST(const string& startingStadium);
    PathReturn calculateDijkstra(string startingStadium, string endingStadium);
    PathReturn calculateDFS(string startingStadium);
    PathReturn calculateCustomTrip(vector<string> trip);
    PathReturn calculateRecursiveTrip(vector<string> trip);

    vector<Stadium> filterStadiums(const vector<Stadium>& stadiumsVect, string search);

    /* ----- PRINT TO TERMINAL -----*/
    /**
     * Prints stadiums to terminal
     */
    void printStadiums() const;

private:
    
    PathReturn shortestTripRecursion(PathReturn& calculatedPath, vector<string>& path,string prevStadium);

    DoubleHashTable<Stadium> stadiums;

    AdjacencyList   adjacencyList;   // list used for dfs
    AdjacencyMatrix adjacencyMatrix; // matrix used for bfs, djikstra, mst

    DatabaseManager databaseManager;

    bool isAdmin;
};

#endif