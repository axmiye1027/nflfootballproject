/**
 * @file databaseManager.h
 * @brief managing database, stadium info, distances and souveniers
 * 
 * Header file for functions to manipulate our database. Gets data from the CSV
 * files and organize it into the database. CSV files are located in the "assets"
 * folder. Does not modify or edit only adds or removes or updates
 */
#pragma once

#include <sqlite3.h>
#include "../models/hash.h"
#include "../includes.h"

/**
 * @brief struct for storing distances between two stadiums
 */
struct Distance 
{
    int    id;
    string locationA;
    string locationB;
    int    distanceKm;
};

/**
 * @brief stores all of a Stadium's information: ID, team name, etc.
 */
struct StadiumStruct
{
    int    stadiumId;
    string teamName;
    string stadiumName;
    int    capacity;
    string location;
    string roofType;
    string surface;
    int    yearOpened;
    string conference;
    string division;
};

/**
 * @brief stores souvenier information 
 */
struct Souvenir
{
    int    stadiumId;
    int    souvenirId;
    string souvenirName;
    double souvenirPrice;

    friend ostream& operator<<(ostream& os, const Souvenir& souvenir) 
    {
        os << souvenir.souvenirName << " : " << souvenir.souvenirPrice;
        return os;
    }

    bool operator==(const string& name) const
    {
        return souvenirName == name;
    }

    bool operator==(const Souvenir& other) const
    {
        return souvenirId == other.souvenirId;
    }

    bool operator!=(const string& name) const
    {
        return souvenirName != name;
    }

    bool operator!=(const Souvenir& other) const
    {
        return souvenirId != other.souvenirId;
    }

    Souvenir() : stadiumId(-1), souvenirId(-1), souvenirName(""), souvenirPrice(0.0) {}
    Souvenir(string name, double price) : stadiumId(-1), souvenirId(-1), souvenirName(name), souvenirPrice(price) {} 
    Souvenir(int stadiumId, string souvenirName, int souvenirPrice) : stadiumId(stadiumId), souvenirName(souvenirName), souvenirPrice(souvenirPrice) {}
};

/**
 * @brief class to manipulate the database. Allows editing and accessing
 */
class DatabaseManager
{
public:
    DatabaseManager(const string& PATH = "./assets/database.db");
    ~DatabaseManager();

    DatabaseManager(const DatabaseManager&)            = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    void initializeDatabase();
    void ensureBidirectionalDistances();


    /****************** EDIT DATABASE ******************/
    int  addDistance(const string& locationA, const string& locationB, double distanceKm);
    bool updateDistance(int distanceId, double distanceKm);
    bool updateDistance(int distanceId, string locationA, string locationB); // bool determines whether to update location A or B in the table
    bool deleteDistance(int distanceId);

    int addStadium( const string& team, 
                    const string& stadiumName,
                    int           capacity, 
                    const string& location,
                    const string& roofType, 
                    const string& surface,
                    int           yearOpened, 
                    const string& conference,
                    const string& division,
                    vector<Souvenir> souvenirs);
    bool updateStadium(int stadiumId, const string& field, const string& value);
    bool updateStadiumInt(int stadiumId, const string& field, int value);
    bool deleteStadium(int stadiumId); 

    int  addSouvenir(int stadiumId, const string& souvenirName, double souvenirPrice);
    bool updateSouvenirName(int souvenirId, const string& souvenirName);
    bool updateSouvenirPrice(int souvenirId, double souvenirPrice);
    bool deleteSouvenir(int souvenirId);
    /***************************************************/

    /***************** GET FROM DATABASE ***************/
    Distance*        getDistance(const string& locationA,const string& locationB);
    vector<Distance> getAllDistances();

    StadiumStruct*        getStadiumById(int stadiumId);
    StadiumStruct*        getStadiumByTeam(const string& team);
    vector<StadiumStruct> getAllStadiums();

    vector<StadiumStruct> getStadiumsByConference(const string& conference);
    vector<StadiumStruct> getStadiumsByDivision(const string& division);

    Souvenir*        getSouvenirById(int souvenirId);
    vector<Souvenir> getStadiumSouvenirs(int stadiumId);
    /***************************************************/

private:
    sqlite3* db;
    string   db_path;

    void executeSQL(const string& sql);
    
    sqlite3_stmt* prepareStatement(const string& sql);
    
    string getColumnText(sqlite3_stmt* stmt, int col);
    int    getColumnInt(sqlite3_stmt* stmt, int col);
};