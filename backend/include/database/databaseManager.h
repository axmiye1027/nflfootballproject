// databaseManager.h

#pragma once

#include <sqlite3.h>
#include "../models/hash.h"
#include "../includes.h"

struct Distance 
{
    int    id;
    string locationA;
    string locationB;
    int    distanceKm;
};

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

    Souvenir() : stadiumId(-1), souvenirId(-1), souvenirName(""), souvenirPrice(0.0) {}
    Souvenir(int stadiumId, string souvenirName, int souvenirPrice) : stadiumId(stadiumId), souvenirName(souvenirName), souvenirPrice(souvenirPrice) {}
};

class DatabaseManager
{
public:
    DatabaseManager(const string& PATH = "../../assets/database.db");
    ~DatabaseManager();

    DatabaseManager(const DatabaseManager&)            = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    void initializeDatabase();


    /****************** EDIT DATABASE ******************/
    int  addDistance(const string& locationA, const string& locationB, double distanceKm);
    bool updateDistance(int distanceId, double distanceKm);
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