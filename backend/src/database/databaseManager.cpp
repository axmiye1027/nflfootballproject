// databaseManager.cpp

#include "database/databaseManager.h"

DatabaseManager::DatabaseManager(const string& PATH) : db_path(PATH)
{
    db = nullptr;

    int rc = sqlite3_open(db_path.c_str(), &db);

    if (rc != SQLITE_OK) 
    {
        string error = "Cannot open database: " + string(sqlite3_errmsg(db));
        sqlite3_close(db);
        throw runtime_error(error);
    }

    initializeDatabase();
}


DatabaseManager::~DatabaseManager()
{
    if (db) 
    {
        sqlite3_close(db);
    }
}

void DatabaseManager::initializeDatabase() 
{
    string createDistances = R"(
        CREATE TABLE IF NOT EXISTS distances (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            locationA TEXT NOT NULL,
            locationB TEXT NOT NULL,
            distanceKm REAL NOT NULL,
            UNIQUE(locationA, locationB)
        )
    )";
    
    string createStadiums = R"(
        CREATE TABLE IF NOT EXISTS stadiums (
            idStadium INTEGER PRIMARY KEY AUTOINCREMENT,
            team TEXT NOT NULL,
            stadiumName TEXT NOT NULL,
            capacity INTEGER,
            location TEXT,
            roofType TEXT,
            surface TEXT,
            yearOpened INTEGER,
            conference TEXT,
            division TEXT
        )
    )";
    
    executeSQL(createDistances);
    executeSQL(createStadiums);
}

// ==================== DISTANCES IMPLEMENTATION ====================

int DatabaseManager::addDistance(const string& locationA, const string& locationB, double distanceKm) 
{
    string sql = "INSERT INTO distances (locationA, locationB, distanceKm) "
                     "VALUES (?, ?, ?)";
    sqlite3_stmt* stmt = prepareStatement(sql);
    
    sqlite3_bind_text(stmt, 1, locationA.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, locationB.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 3, distanceKm);
    
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) 
    {
        throw runtime_error("Failed to insert distance");
    }
    
    return static_cast<int>(sqlite3_last_insert_rowid(db));
}

Distance* DatabaseManager::getDistance(const string& locationA, const string& locationB) 
{
    string sql = "SELECT * FROM distances WHERE "
                     "(locationA = ? AND locationB = ?) OR "
                     "(locationA = ? AND locationB = ?)";
    sqlite3_stmt* stmt = prepareStatement(sql);
    
    sqlite3_bind_text(stmt, 1, locationA.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, locationB.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, locationB.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, locationA.c_str(), -1, SQLITE_TRANSIENT);
    
    Distance* result = nullptr;
    if (sqlite3_step(stmt) == SQLITE_ROW) 
    {
        result = new Distance();
        result->id         = getColumnInt(stmt, 0);
        result->locationA  = getColumnText(stmt, 1);
        result->locationB  = getColumnText(stmt, 2);
        result->distanceKm = sqlite3_column_double(stmt, 3);
    }
    
    sqlite3_finalize(stmt);
    return result;
}

vector<Distance> DatabaseManager::getAllDistances() 
{
    string sql = "SELECT * FROM distances";
    sqlite3_stmt* stmt = prepareStatement(sql);
    
    vector<Distance> distances;
    while (sqlite3_step(stmt) == SQLITE_ROW) 
    {
        Distance d;
        d.id = getColumnInt(stmt, 0);
        d.locationA = getColumnText(stmt, 1);
        d.locationB = getColumnText(stmt, 2);
        d.distanceKm = sqlite3_column_double(stmt, 3);
        distances.push_back(d);
    }
    
    sqlite3_finalize(stmt);
    return distances;
}

bool DatabaseManager::updateDistance(int distanceId, double distanceKm) 
{
    string sql = "UPDATE distances SET distanceKm = ? WHERE id = ?";
    sqlite3_stmt* stmt = prepareStatement(sql);
    
    sqlite3_bind_double(stmt, 1, distanceKm);
    sqlite3_bind_int(stmt, 2, distanceId);
    
    sqlite3_step(stmt);
    int changes = sqlite3_changes(db);
    sqlite3_finalize(stmt);
    
    return changes > 0;
}

bool DatabaseManager::deleteDistance(int distanceId) 
{
    string sql = "DELETE FROM distances WHERE id = ?";
    sqlite3_stmt* stmt = prepareStatement(sql);
    
    sqlite3_bind_int(stmt, 1, distanceId);
    
    sqlite3_step(stmt);
    int changes = sqlite3_changes(db);
    sqlite3_finalize(stmt);
    
    return changes > 0;
}

// ==================== STADIUMS IMPLEMENTATION ====================

int DatabaseManager::addStadium(const string& team, 
                                const string& stadiumName,
                                int           capacity, 
                                const string& location,
                                const string& roofType, 
                                const string& surface,
                                int           yearOpened, 
                                const string& conference,
                                const string& division) 
{
    string sql = "INSERT INTO stadiums (team, stadiumName, capacity, location, "
                     "roofType, surface, yearOpened, conference, division) "
                     "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";
    sqlite3_stmt* stmt = prepareStatement(sql);
    
    sqlite3_bind_text(stmt, 1, team.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, stadiumName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, capacity);
    sqlite3_bind_text(stmt, 4, location.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, roofType.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, surface.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 7, yearOpened);
    sqlite3_bind_text(stmt, 8, conference.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 9, division.c_str(), -1, SQLITE_TRANSIENT);
    
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) 
    {
        throw runtime_error("Failed to insert stadium");
    }
    
    return static_cast<int>(sqlite3_last_insert_rowid(db));
}

StadiumStruct* DatabaseManager::getStadiumById(int stadiumId)
{
    string sql = "SELECT * FROM stadiums WHERE idStadium = ?";
    sqlite3_stmt* stmt = prepareStatement(sql);
    
    sqlite3_bind_int(stmt, 1, stadiumId);
    
    StadiumStruct* result = nullptr;
    if (sqlite3_step(stmt) == SQLITE_ROW) 
    {
        result = new StadiumStruct();

        result->idStadium   = getColumnInt(stmt, 0);
        result->team        = getColumnText(stmt, 1);
        result->stadiumName = getColumnText(stmt, 2);
        result->capacity    = getColumnInt(stmt, 3);
        result->location    = getColumnText(stmt, 4);
        result->roofType    = getColumnText(stmt, 5);
        result->surface     = getColumnText(stmt, 6);
        result->yearOpened  = getColumnInt(stmt, 7);
        result->conference  = getColumnText(stmt, 8);
        result->division    = getColumnText(stmt, 9);
    }
    
    sqlite3_finalize(stmt);
    return result;
}

StadiumStruct* DatabaseManager::getStadiumByTeam(const string& team) 
{
    string sql = "SELECT * FROM stadiums WHERE team = ?";
    sqlite3_stmt* stmt = prepareStatement(sql);
    
    sqlite3_bind_text(stmt, 1, team.c_str(), -1, SQLITE_TRANSIENT);
    
    StadiumStruct* result = nullptr;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        result = new StadiumStruct();

        result->idStadium    = getColumnInt(stmt, 0);
        result->team         = getColumnText(stmt, 1);
        result->stadiumName  = getColumnText(stmt, 2);
        result->capacity     = getColumnInt(stmt, 3);
        result->location     = getColumnText(stmt, 4);
        result->roofType     = getColumnText(stmt, 5);
        result->surface      = getColumnText(stmt, 6);
        result->yearOpened   = getColumnInt(stmt, 7);
        result->conference   = getColumnText(stmt, 8);
        result->division     = getColumnText(stmt, 9);
    }
    
    sqlite3_finalize(stmt);
    return result;
}

vector<StadiumStruct> DatabaseManager::getAllStadiums() 
{
    string sql = "SELECT * FROM stadiums";
    sqlite3_stmt* stmt = prepareStatement(sql);
    
    vector<StadiumStruct> stadiums;
    while (sqlite3_step(stmt) == SQLITE_ROW) 
    {
        StadiumStruct s;

        s.idStadium   = getColumnInt(stmt, 0);
        s.team        = getColumnText(stmt, 1);
        s.stadiumName = getColumnText(stmt, 2);
        s.capacity    = getColumnInt(stmt, 3);
        s.location    = getColumnText(stmt, 4);
        s.roofType    = getColumnText(stmt, 5);
        s.surface     = getColumnText(stmt, 6);
        s.yearOpened  = getColumnInt(stmt, 7);
        s.conference  = getColumnText(stmt, 8);
        s.division    = getColumnText(stmt, 9);

        stadiums.push_back(s);
    }
    
    sqlite3_finalize(stmt);
    return stadiums;
}

vector<StadiumStruct> DatabaseManager::getStadiumsByConference(const string& conference) 
{
    string sql = "SELECT * FROM stadiums WHERE conference = ?";
    sqlite3_stmt* stmt = prepareStatement(sql);
    
    sqlite3_bind_text(stmt, 1, conference.c_str(), -1, SQLITE_TRANSIENT);
    
    vector<StadiumStruct> stadiums;
    while (sqlite3_step(stmt) == SQLITE_ROW) 
    {
        StadiumStruct s;

        s.idStadium   = getColumnInt(stmt, 0);
        s.team        = getColumnText(stmt, 1);
        s.stadiumName = getColumnText(stmt, 2);
        s.capacity    = getColumnInt(stmt, 3);
        s.location    = getColumnText(stmt, 4);
        s.roofType    = getColumnText(stmt, 5);
        s.surface     = getColumnText(stmt, 6);
        s.yearOpened  = getColumnInt(stmt, 7);
        s.conference  = getColumnText(stmt, 8);
        s.division    = getColumnText(stmt, 9);

        stadiums.push_back(s);
    }
    
    sqlite3_finalize(stmt);
    return stadiums;
}

vector<StadiumStruct> DatabaseManager::getStadiumsByDivision(const string& division) 
{
    string sql = "SELECT * FROM stadiums WHERE division = ?";
    sqlite3_stmt* stmt = prepareStatement(sql);
    
    sqlite3_bind_text(stmt, 1, division.c_str(), -1, SQLITE_TRANSIENT);
    
    vector<StadiumStruct> stadiums;
    while (sqlite3_step(stmt) == SQLITE_ROW) 
    {
        StadiumStruct s;

        s.idStadium   = getColumnInt(stmt, 0);
        s.team        = getColumnText(stmt, 1);
        s.stadiumName = getColumnText(stmt, 2);
        s.capacity    = getColumnInt(stmt, 3);
        s.location    = getColumnText(stmt, 4);
        s.roofType    = getColumnText(stmt, 5);
        s.surface     = getColumnText(stmt, 6);
        s.yearOpened  = getColumnInt(stmt, 7);
        s.conference  = getColumnText(stmt, 8);
        s.division    = getColumnText(stmt, 9);

        stadiums.push_back(s);
    }
    
    sqlite3_finalize(stmt);
    return stadiums;
}

bool DatabaseManager::updateStadium(int stadiumId, const string& field, const string& value) 
{
    string sql = "UPDATE stadiums SET " + field + " = ? WHERE idStadium = ?";
    sqlite3_stmt* stmt = prepareStatement(sql);
    
    sqlite3_bind_text(stmt, 1, value.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, stadiumId);
    
    sqlite3_step(stmt);
    int changes = sqlite3_changes(db);
    sqlite3_finalize(stmt);
    
    return changes > 0;
}

bool DatabaseManager::updateStadiumInt(int stadiumId, const string& field, int value) 
{
    string sql = "UPDATE stadiums SET " + field + " = ? WHERE idStadium = ?";
    sqlite3_stmt* stmt = prepareStatement(sql);
    
    sqlite3_bind_int(stmt, 1, value);
    sqlite3_bind_int(stmt, 2, stadiumId);
    
    sqlite3_step(stmt);
    int changes = sqlite3_changes(db);
    sqlite3_finalize(stmt);
    
    return changes > 0;
}

bool DatabaseManager::deleteStadium(int stadiumId) 
{
    string sql = "DELETE FROM stadiums WHERE idStadium = ?";
    sqlite3_stmt* stmt = prepareStatement(sql);
    
    sqlite3_bind_int(stmt, 1, stadiumId);
    
    sqlite3_step(stmt);
    int changes = sqlite3_changes(db);
    sqlite3_finalize(stmt);
    
    return changes > 0;
}

void DatabaseManager::executeSQL(const string& sql) 
{
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    
    if (rc != SQLITE_OK)
    {
        string error = "SQL error: " + string(errMsg);
        sqlite3_free(errMsg);
        throw runtime_error(error);
    }
}

sqlite3_stmt* DatabaseManager::prepareStatement(const string& sql) 
{
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) 
    {
        throw runtime_error("Failed to prepare statement: " + string(sqlite3_errmsg(db)));
    }
    return stmt;
}

string DatabaseManager::getColumnText(sqlite3_stmt* stmt, int col)
{
    const unsigned char* text = sqlite3_column_text(stmt, col);
    return text ? string(reinterpret_cast<const char*>(text)) : "";
}

int DatabaseManager::getColumnInt(sqlite3_stmt* stmt, int col) 
{
    return sqlite3_column_int(stmt, col);
}