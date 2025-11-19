// databaseManager.h

#include "sqlite3.h"
#include "includes.h"

struct Distance 
{
    int    id;
    string locationA;
    string locationB;
    double distanceKm;
};

struct Stadium 
{
    int    idStadium;
    string team;
    string stadiumName;
    int    capacity;
    string location;
    string roofType;
    string surface;
    int    yearOpened;
    string conference;
    string division;
};

class DatabaseManager
{
public:
    DatabaseManager(const string& PATH = "backend/assets/database.db");
    ~DatabaseManager();

    DatabaseManager(const NFLDatabaseManager&)            = delete;
    DatabaseManager& operator=(const NFLDatabaseManager&) = delete;

    void initializeDatabase();

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
                    const string& division);
    bool updateStadium(int stadiumId, const string& field, const string& value);
    bool updateStadiumInt(int stadiumId, const string& field, int value);
    bool deleteStadium(int stadiumId); 

    Distance*        getDistance(const string& locationA,const string& locationB);
    vector<Distance> getAllDistances();

    Stadium*        getStadiumById(int stadiumId);
    Stadium*        getStadiumByTeam(const string& team);
    vector<Stadium> getAllStadiums();

    vector<Stadium> getStadiumsByConference(const string& conference);
    vector<Stadium> getStadiumsByDivision(const string& division);

private:
    sqlite3* db;
    string   db_path;

    void executeSQL(const string& sql);
    
    sqlite3_stmt* prepareStatement(const string& sql);
    
    string getColumnText(sqlite3_stmt* stmt, int col);
    int    getColumnInt(sqlite3_stmt* stmt, int col);
};