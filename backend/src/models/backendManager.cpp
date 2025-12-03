/**
 * @file backendManager.cpp
 * @brief implmenetaion for backendManager
 */

#include "../../include/models/backendManager.h"
#include "crow.h"

/**
 * @brief constuctor, assumes user is not an adimin
 */
BackendManager::BackendManager() : isAdmin{false}
{
    populateStadiums();
    populateDistances();
    // printStadiums();

    //adjacencyMatrix.printMatrix();
    adjacencyMatrix.bfs("State Farm Stadium");

    getStadiumsByDivision(getStadiumsAsVector(),"NFC North");

    //adjacencyMatrix.Dijkstra("State Farm Stadium");
    //adjacencyMatrix.mst("State Farm Stadium");

    //adjacencyList.dfs("State Farm Stadium");
}

/**
 * @brief destructor
 */
BackendManager::~BackendManager()
{

}

/**
 * @param username user input for username
 * @param password user input for password
 * @return boolean, true or false
 * 
 * User inputs 2 strings, a username and password and checks if they're valid
 * credentials. If valid returns true, invalid returns false
 */
bool BackendManager::login(string username, string password)
{
    if(username == "admin" && password == "admin")
    {
        cout << "Login successful." << endl;
        isAdmin = true;
        return true;
    }
    cout << "Invalid credentials." << endl;
    isAdmin = false;
    return false;
}

/**
 * 
 */
bool BackendManager::importStadiums(const string& json, std::string &error_out)
{
    // Expect `json` to be the JSON contents (not a file path)
    auto j = crow::json::load(json);
    if (!j)
    {
        error_out = "Invalid JSON";
        return false;
    }

    try {
        // Validate required keys
        const std::vector<std::string> requiredKeys = {"team", "stadium", "capacity", "location", "roof", "surface", "year", "conference", "division"};
        for (const auto &k : requiredKeys) {
            if (!j.has(k)) {
                error_out = string("Missing required key: ") + k;
                return false;
            }
        }

        std::string team      = j["team"].s();
        std::string stadium   = j["stadium"].s();
        int capacity          = j["capacity"].i();
        std::string location  = j["location"].s();
        std::string roofType  = j["roof"].s();
        std::string surface   = j["surface"].s();
        int yearOpened        = j["year"].i();
        std::string conference= j["conference"].s();
        std::string division  = j["division"].s();

        // databaseManager.addStadium expects souvenir vector as last argument
        std::vector<Souvenir> emptySouvenirs;
        int stadiumId = databaseManager.addStadium(team, stadium, capacity, location,
                                roofType, surface, yearOpened, conference, division, emptySouvenirs);

        if (j.has("distances") && j["distances"].t() == crow::json::type::List) {
            auto &arr = j["distances"];
            for (size_t i = 0; i < arr.size(); ++i) {
                const auto &d = arr[i];
                if (!d.has("locationA") || !d.has("locationB") || !d.has("distanceKm"))
                    continue; // skip invalid entry

                std::string locA = d["locationA"].s();
                std::string locB = d["locationB"].s();
                double distKm = 0.0;
                try {
                    distKm = d["distanceKm"].d();
                } catch (...) {
                    distKm = static_cast<double>(d["distanceKm"].i());
                }

                // databaseManager is a member; call addDistance
                try {
                    databaseManager.addDistance(locA, locB, distKm);
                } catch (const std::exception &ex) {
                    // continue on error for individual distances
                }
            }
        }

        // Refresh in-memory structures so routes that read `stadiums` reflect the new data
        try {
            populateStadiums();
            populateDistances();
        } catch (...) {
            // non-fatal: log and continue
            std::cerr << "[importStadiums] Warning: failed to refresh in-memory data after import" << std::endl;
        }

        return true;
    }
    catch (exception &ex) {
        error_out = string("Error! ") + ex.what();
        return false;
    }
}

/**
 * 
 */
void BackendManager::populateStadiums() 
{
    stadiums.clearTable();

    cout << "[BackendManager::populateStadiums()]" << endl;

    vector<StadiumStruct> stadiumsVect = databaseManager.getAllStadiums();

    for (int i = 0; i < stadiumsVect.size(); ++i)
    { 
        vector<Souvenir> souvenirsList = databaseManager.getStadiumSouvenirs(stadiumsVect[i].stadiumId);
        DoubleHashTable<Souvenir> souvenirs;

        RoofType roof;
        if (stadiumsVect[i].roofType == "Open")
        {
            roof = OPEN;
        }
        else if (stadiumsVect[i].roofType == "Retractable")
        {
            roof = RETRACTABLE;
        }
        else if (stadiumsVect[i].roofType == "Fixed")
        {
            roof = FIXED_ROOF;
        }

        for (int j = 0; j < souvenirsList.size(); ++j)
        {
            souvenirs.insert(keyInput(souvenirsList[j], souvenirsList[j].souvenirId));
        }

        Stadium stadium(
            stadiumsVect[i].teamName,
            stadiumsVect[i].stadiumName,
            stadiumsVect[i].capacity,
            stadiumsVect[i].location,
            roof,
            stadiumsVect[i].surface,
            stadiumsVect[i].yearOpened,
            stadiumsVect[i].conference,
            stadiumsVect[i].division,
            souvenirs
        );
        stadium.setStadiumId(stadiumsVect[i].stadiumId);

        stadiums.insert(keyInput(stadium, stadiumsVect[i].stadiumId));
    }
}


void BackendManager::populateDistances()
{
    vector<Distance> distances = databaseManager.getAllDistances();

    // AdjacencyMatrix
    adjacencyMatrix.setDistances(distances);
    adjacencyMatrix.populateVertices(stadiums);

    // AdjacencyList
    vector<edge> edges;
    for (int i = 0; i < distances.size(); ++i)
    {
        edge edge(distances[i].locationA, distances[i].locationB, distances[i].distanceKm);

        edges.push_back(edge);
    }

    adjacencyList.populateList(edges);
}


void BackendManager::printStadiums() const
{
    stadiums.printTable();
}


void BackendManager::addStadium(string teamName, string stadiumName, int capacity, string location, RoofType roofType, string surface,
            int yearOpened, string conference, string division, vector<Souvenir> souvenirList)
{
    string roof;

    switch (roofType)
    {
        case OPEN:        roof = "Open";
        break;

        case RETRACTABLE: roof = "Retractable";
        break;

        case FIXED_ROOF:  roof = "Fixed";
        break;
    }

    databaseManager.addStadium(teamName, stadiumName, capacity, location, roof, surface, yearOpened, conference, division, souvenirList);

}

bool BackendManager::updateStadium(int stadiumId, string teamName, string stadiumName, int capacity, string location, string roofType, string surface,
            int yearOpened, string conference, string division)
{
    try 
    {
        string oldStadiumName      = getStadiumById(getStadiumsAsVector(),stadiumId).getStadiumName();
        vector<Distance> distances = adjacencyMatrix.getDistanceVector();

        // goes through the whole vector and finds the distance name to update
        for(int i = 0; i < distances.size(); ++i)
        {
            bool   needsUpdate  = false;
            string newLocationA = distances[i].locationA;
            string newLocationB = distances[i].locationB;

            if (distances[i].locationA == oldStadiumName)
            {
                newLocationA = stadiumName;
                needsUpdate = true;
            }
            
            if (distances[i].locationB == oldStadiumName)
            {
                newLocationB = stadiumName;
                needsUpdate = true;
            }

            if (needsUpdate)
            {
                bool distanceUpdated = databaseManager.updateDistance(distances[i].id, newLocationA, newLocationB);
                
                if (!distanceUpdated)
                {
                    cout << "[BackendManager::updateStadium] Failed to update distance ID: " << distances[i].id << endl;
                }
            }
        }

        databaseManager.updateStadium(stadiumId,    "team",         teamName);
        databaseManager.updateStadium(stadiumId,    "stadium_name", stadiumName);
        databaseManager.updateStadiumInt(stadiumId, "capacity",     capacity);
        databaseManager.updateStadium(stadiumId,    "location",     location);
        databaseManager.updateStadium(stadiumId,    "roof_type",    roofType);
        databaseManager.updateStadium(stadiumId,    "surface",      surface);
        databaseManager.updateStadiumInt(stadiumId, "opened_year",  yearOpened);
        databaseManager.updateStadium(stadiumId,    "conference",   conference);
        databaseManager.updateStadium(stadiumId,    "division",     division);

        return true;
    }
    catch (...) 
    {
        cerr << "[BackendManager::updateStadium] error." << endl;
        return false;
    }
}

void BackendManager::addTeam() {}

void BackendManager::addSouvenir(int stadiumId, const string& souvenirName, double souvenirPrice)
{
    databaseManager.addSouvenir(stadiumId, souvenirName, souvenirPrice);
}

void BackendManager::removeSouvenir(int stadiumId,const string&  name)
{
    databaseManager.deleteSouvenir(stadiums.get(stadiumId).getSouvenirId(name));
}

void BackendManager::modifySouvenirName(int stadiumId,const string&  oldName,const string& newName)
{
    databaseManager.updateSouvenirName(stadiums.get(stadiumId).getSouvenirId(oldName),newName);
}

void BackendManager::modifySouvenirPrice(int stadiumId, const string& name, double souvenirPrice)
{
    databaseManager.updateSouvenirPrice(stadiums.get(stadiumId).getSouvenirId(name),souvenirPrice);
}


vector<Stadium> BackendManager::getStadiumsAsVector()
{
    vector<Stadium> stadiumsVect;

    for (int i = 0; i < stadiums.getCount(); ++i)
    {
        stadiumsVect.push_back(stadiums.get(i + 1));
    }

    return stadiumsVect;
}

template<typename Compare>
vector<Stadium> BackendManager::sortStadiums(const vector<Stadium>& stadiumsVect, Compare compare)
{
    priority_queue<Stadium, vector<Stadium>, Compare> pq(compare);

    for(int i = 0; i < stadiumsVect.size(); ++i)
    {
        pq.push(stadiumsVect[i]);
    }
    
    vector<Stadium> sorted;
    sorted.reserve(stadiumsVect.size());
    
    while(!pq.empty())
    {
        sorted.push_back(pq.top());
        pq.pop();
    }
    
    return sorted;
}

vector<Stadium> BackendManager::sortStadiumsByCapacity(const vector<Stadium>& stadiums)
{
    return sortStadiums(stadiums, CompareByCapacity());
}

vector<Stadium> BackendManager::sortStadiumsByDateOpened(const vector<Stadium>& stadiums)
{
    return sortStadiums(stadiums, CompareByYearOpened());
}

vector<Stadium> BackendManager::sortStadiumsByStadiumName(const vector<Stadium>& stadiums)
{
    return sortStadiums(stadiums, CompareByStadiumName());
}

vector<Stadium> BackendManager::sortStadiumsByTeam(const vector<Stadium>& stadiums, string teamName)
{
    return sortStadiums(stadiums, CompareByStadiumName());
}

vector<Stadium> BackendManager::getStadiumsByRoofType(const vector<Stadium>& stadiumsVect, string roofType)
{
    //transform(roofType.begin(), roofType.end(), roofType.begin(), ::toupper);

    vector<Stadium> roofTypes;

    for(int i = 0; i < stadiumsVect.size(); ++i)
    {
        string stadiumRoof;

        switch(stadiumsVect[i].getRoofType())
        {
            case OPEN:
                stadiumRoof = "Open";
            break;

            case FIXED_ROOF:
                stadiumRoof = "Fixed";
            break;

            case RETRACTABLE:
                stadiumRoof = "Retractable";
            break;

            default:
                throw runtime_error("Unrecognized roof type found");
        }

        if(stadiumRoof == roofType)
        {
            roofTypes.push_back(stadiumsVect[i]);
        }
    }

    return roofTypes;
}

Stadium BackendManager::getStadiumById(const vector<Stadium>& stadiumsVect,int id)
{
    for(int i = 0; i < stadiumsVect.size(); ++i)
    {
        if(stadiumsVect[i].getStadiumId() == id)
        {
            return stadiumsVect[i];
        }
    }

    throw runtime_error("Stadium not found");
}

Stadium BackendManager::getStadiumByName(const vector<Stadium>& stadiumsVect,string stadiumName)
{
    for(int i = 0; i < stadiumsVect.size(); ++i)
    {
        if(stadiumsVect[i].getStadiumName() == stadiumName)
        {
            return stadiumsVect[i];
        }
    }

    throw runtime_error("Stadium not found");
}

vector<Stadium> BackendManager::getStadiumsByConference(const vector<Stadium>& stadiumsVect,string conference)
{
    transform(conference.begin(), conference.end(), conference.begin(), ::toupper);

    vector<Stadium> stadiums;

    for(int i = 0; i < stadiumsVect.size(); ++i)
    {
        if(stadiumsVect[i].getDivision().substr(0, 3) == conference)
        {
            stadiums.push_back(stadiumsVect[i]);
        }
    }

    return stadiums;
}

vector<Stadium> BackendManager::getStadiumsByDivision(const vector<Stadium>& stadiumsVect,string divison)
{
    //transform(divison.begin(), divison.end(), divison.begin(), ::toupper);

    vector<Stadium> stadiums;

    for(int i = 0; i < stadiumsVect.size(); ++i)
    {
        //cout << stadiumsVect[i].getDivision() << endl;

        if(stadiumsVect[i].getDivision() == divison)
        {
            stadiums.push_back(stadiumsVect[i]);
        }
    }

    return stadiums;
}

vector<Stadium> getStadiumsByGrass(const vector<Stadium>& stadiumsVect,string grassType)
{
    transform(grassType.begin(), grassType.end(), grassType.begin(), ::toupper);

    vector<Stadium> stadiums;

    for(int i = 0; i < stadiumsVect.size(); ++i)
    {
        if(stadiumsVect[i].getSurfaceType() == grassType)
        {
            stadiums.push_back(stadiumsVect[i]);
        }
    }

    return stadiums;
}

DoubleHashTable<Souvenir> BackendManager::getTeamSouvenirs(string teamName)
{
    vector<Stadium> stadiums = getStadiumsAsVector();

    for(int i = 0; i < stadiums.size(); ++i)
    {
        if(stadiums[i].getTeamName() == teamName)
        {
            return stadiums[i].getSouvenirList();
        }
    }

    throw runtime_error("Team '" + teamName + "' not found");
}

int BackendManager::totalStadiumCapacity(const vector<Stadium>& stadiumsVect)
{
    set<string> seenStadiums;
    int totalCapacity = 0;

    for(int i = 0; i < stadiumsVect.size(); ++i)
    {
        string name = stadiumsVect[i].getStadiumName();
        
        if(seenStadiums.find(name) == seenStadiums.end())
        {
            seenStadiums.insert(name);
            totalCapacity += stadiumsVect[i].getCapacity();
        }
    }

    return totalCapacity;
}

vector<Stadium> BackendManager::filterStadiums(const vector<Stadium>& stadiumsVect, string search)
{
    transform(search.begin(), search.end(), search.begin(), ::tolower); // tolower the search string

    vector<Stadium> output;

    for (int i = 0; i < stadiumsVect.size(); ++i)
    {
        string teamName    = stadiumsVect[i].getTeamName();
        string stadiumName = stadiumsVect[i].getStadiumName();

        // lowercase both for case-insensitive search
        transform(teamName.begin(),    teamName.end(),    teamName.begin(),    ::tolower);
        transform(stadiumName.begin(), stadiumName.end(), stadiumName.begin(), ::tolower);

        if (teamName.find(search)    != string::npos || // True if search is not in string
            stadiumName.find(search) != string::npos)
            {
                output.push_back(stadiumsVect[i]);
            }
    }

    return output;
}


int BackendManager::calculateBFS(string startingCity)
{
    transform(startingCity.begin(), startingCity.end(), startingCity.begin(), ::tolower);

    return adjacencyMatrix.bfs(startingCity);
}


int BackendManager::calculateMST(string startingCity)
{
    transform(startingCity.begin(), startingCity.end(), startingCity.begin(), ::tolower);

    return adjacencyMatrix.mst(startingCity);
}