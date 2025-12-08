/**
 * @file backendManager.cpp
 * @brief implmenetaion for backendManager
 */

#include "../../include/models/backendManager.h"

/**
 * @brief constuctor, assumes user is not an adimin
 */
BackendManager::BackendManager() : isAdmin{false}
{
    populateStadiums();
    populateDistances();
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
void BackendManager::importStadiums(const string& json)
{
    // try {
    //     std::string team      = j["team"].s();
    //     std::string stadium   = j["stadium"].s();
    //     int capacity          = j["capacity"].i();
    //     std::string location  = j["location"].s();
    //     std::string roofType  = j["roof"].s();
    //     std::string surface   = j["surface"].s();
    //     int yearOpened        = j["year"].i();
    //     std::string conference= j["conference"].s();
    //     std::string division  = j["division"].s();

    //     int stadiumId = dbManager.addStadium(team, stadium, capacity, location,
    //                             roofType, surface, yearOpened, conference, division);
    // }
    // catch {

    // }

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


int BackendManager::addStadium(string teamName, string stadiumName, int capacity, string location, RoofType roofType, string surface,
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

    // Return the new stadium ID from the database
    int newStadiumId = databaseManager.addStadium(teamName, stadiumName, capacity, location, roof, surface, yearOpened, conference, division, souvenirList);

    return newStadiumId; // Return the ID instead of just true
}

bool BackendManager::updateStadium(int stadiumId, string teamName, string stadiumName, int capacity, string location, string roofType, string surface,
            int yearOpened, string conference, string division, vector<Souvenir> souvenirs)
{
    try 
    {
        string oldStadiumName      = getStadiumById(getStadiumsAsVector(),stadiumId).getStadiumName();
        vector<Distance> distances = adjacencyMatrix.getDistanceVector();

        // goes through the whole vector and finds the distance name to update
        for (int i = 0; i < distances.size(); ++i)
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

        for (int i = 0; i < souvenirs.size(); ++i)
        {
            if (souvenirs[i].souvenirId == -1)
            {
                databaseManager.addSouvenir(souvenirs[i].stadiumId, souvenirs[i].souvenirName, souvenirs[i].souvenirPrice);
            }
            else 
            {
                databaseManager.updateSouvenirName(souvenirs[i].souvenirId, souvenirs[i].souvenirName);
                databaseManager.updateSouvenirPrice(souvenirs[i].souvenirId, souvenirs[i].souvenirPrice);
            }

        }

        return true;
    }
    catch (...) 
    {
        cerr << "[BackendManager::updateStadium] error." << endl;
        return false;
    }
}

bool BackendManager::deleteStadium(int stadiumId)
{
    string oldStadiumName      = getStadiumById(getStadiumsAsVector(),stadiumId).getStadiumName();
    vector<Distance> distances = adjacencyMatrix.getDistanceVector();

    // goes through the whole vector and finds the distance name to delete
    for (int i = 0; i < distances.size(); ++i)
    {
        bool   needsUpdate  = false;

        if (distances[i].locationA == oldStadiumName || distances[i].locationB == oldStadiumName)
        {
            needsUpdate = true;
        }

        if (needsUpdate)
        {
            bool distanceUpdated = databaseManager.deleteDistance(distances[i].id);
            
            if (!distanceUpdated)
            {
                cout << "[BackendManager::updateStadium] Failed to delete distance ID: " << distances[i].id << endl;
            }
        }
    }

    databaseManager.deleteStadium(stadiumId);

    return true;
}


void BackendManager::addTeam() {}

void BackendManager::addSouvenir(int stadiumId, const string& souvenirName, double souvenirPrice)
{
    databaseManager.addSouvenir(stadiumId, souvenirName, souvenirPrice);
}

void BackendManager::removeSouvenir(int stadiumId, const string&  name)
{
    databaseManager.deleteSouvenir(stadiums.get(stadiumId).getSouvenirId(name));
}

void BackendManager::removeSouvenir(int souvenirId)
{
    databaseManager.deleteSouvenir(souvenirId);
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
    return sortStadiums(stadiums, CompareByTeamName());
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

bool BackendManager::addDistance(Distance distance)
{
    databaseManager.addDistance(distance.locationA, distance.locationB, distance.distanceKm);
    //adjacencyMatrix.addEdge(distance);

    return true;
}

Stadium BackendManager::getStadiumById(int id)
{
    vector<Stadium> stadiumsVect = stadiums.getValues();

    for(int i = 0; i < stadiumsVect.size(); ++i)
    {
        if(stadiumsVect[i].getStadiumId() == id)
        {
            return stadiumsVect[i];
        }
    }

    throw runtime_error("Stadium not found");
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

Stadium BackendManager::getStadiumByName(string stadiumName)
{
    vector<Stadium> stadiumsVect = stadiums.getValues();

    for(int i = 0; i < stadiumsVect.size(); ++i)
    {
        if(stadiumsVect[i].getStadiumName() == stadiumName)
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

vector<Stadium> BackendManager::getStadiumsBySurface(const vector<Stadium>& stadiumsVect,string surface)
{
    //transform(surface.begin(), surface.end(), surface.begin(), ::toupper);

    vector<Stadium> stadiums;

    for(int i = 0; i < stadiumsVect.size(); ++i)
    {
        if(stadiumsVect[i].getSurfaceType() == surface)
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


int BackendManager::calculateBFS(const string& startingCity)
{
    return adjacencyMatrix.bfs(startingCity);
}


int BackendManager::calculateMST(const string& startingCity)
{
    return adjacencyMatrix.mst(startingCity);
}

PathReturn BackendManager::calculateDijkstra(string startingStadium, string endingStadium)
{
    cout << "[BackendManager::calculateDijkstra]\n";
    auto dijkstraList = adjacencyMatrix.dijkstra(startingStadium);

    for (int i = 0; i < dijkstraList.size(); ++i)
    {
        if(!dijkstraList[i].path.empty() &&
            dijkstraList[i].path.front() == startingStadium && 
            dijkstraList[i].path.back()  == endingStadium)
        {
            cout << "[BackendManager::calculateDijkstra]: return dijkstraList[i]\n";
            return dijkstraList[i];
        }
    }

    throw runtime_error("Path not found");
}

PathReturn BackendManager::calculateDFS(string startingStadium)
{
    adjacencyList.dfs(startingStadium);

    vector<AdjacencyNode> path = adjacencyList.returnPath();
    PathReturn convertedPaths; 

    // converts the path returned by dfs to PathReturn struct
    for(int i = 0; i < path.size(); ++i)
    {
        convertedPaths.path.push_back(path[i].cityName);
        convertedPaths.distanceTraveled += path[i].distanceFromLast;
    }

    return convertedPaths;
}

PathReturn BackendManager::calculateCustomTrip(vector<string> trip)
{
    PathReturn path;
    path.path = trip;

    if (trip.size() < 2)
    {
        return path;
    }

    for (int i = 0; i < trip.size() - 1; ++i)
    {
        auto dijkstraList = adjacencyMatrix.dijkstra(trip[i]);

        bool foundPath = false;
        for (int j = 0; j < dijkstraList.size(); ++j)
        {
            if (!dijkstraList[j].path.empty() && dijkstraList[j].path.back() == trip[i+1])
            {
                path.distanceTraveled += dijkstraList[j].distanceTraveled;
                foundPath = true;
                break;
            }
        }

        if (!foundPath)
        {
            cerr << "Error: No path found from " << trip[i] 
                 << " to " << trip[i+1] << endl;
        }
    }

    return path;
}

///NEW - For dropdown fix
vector<Stadium> BackendManager::getStadiumsByTeamName(const vector<Stadium>& stadiumsVect, string teamName)
{
    vector<Stadium> output;
    for (auto& s : stadiumsVect)
        if (s.getTeamName() == teamName)
            output.push_back(s);
    return output;
}
///NEW - For dropdown fix
vector<Stadium> BackendManager::getStadiumsByStadiumName(const vector<Stadium>& stadiumsVect, const string& stadiumName)
{
    vector<Stadium> result;
    for (auto& s : stadiumsVect)
        if (s.getStadiumName() == stadiumName)
            result.push_back(s);
    return result;
}

PathReturn BackendManager::calculateRecursiveTrip(vector<string> trip)
{
    PathReturn recursivePath;
    
    if (trip.empty())
    {
        return recursivePath;
    }
    
    string startingStadium = trip[0];
    recursivePath.path.push_back(startingStadium);
    trip.erase(trip.begin());              

    return shortestTripRecursion(recursivePath, trip, startingStadium);
}

PathReturn BackendManager::shortestTripRecursion(PathReturn& calculatedPath, vector<string>& path, string prevStadium)
{
    if (path.empty())
    {
        return calculatedPath;
    }

    auto dijkstraList = adjacencyMatrix.dijkstra(prevStadium);
    int closestDist = 99999999;
    string nextStadium;
    bool foundStadium = false;

    for (int j = 0; j < dijkstraList.size(); ++j)
    {
        if (dijkstraList[j].path.empty())
        {
            continue;
        }
        
        string stadiumName = dijkstraList[j].path.back();

        if (find(path.begin(), path.end(), stadiumName) != path.end())
        {
            if (dijkstraList[j].distanceTraveled < closestDist)
            {
                closestDist = dijkstraList[j].distanceTraveled;
                nextStadium = stadiumName;
                foundStadium = true;
            }
        }
    }

    if (!foundStadium)
    {
        cerr << "Error: No reachable stadium found from " << prevStadium << endl;
        return calculatedPath;
    }

    calculatedPath.path.push_back(nextStadium);
    calculatedPath.distanceTraveled += closestDist;

    path.erase(find(path.begin(), path.end(), nextStadium));

    return shortestTripRecursion(calculatedPath, path, nextStadium);
}

void BackendManager::addPathToCart(vector<string> path)
{
    vector<Stadium> stadiumsVect = getStadiumsAsVector();
    vector<Stadium> pathStadiums;

    for (int i = 0; i < path.size(); ++i)
    {
        for (int j = 0; j < stadiumsVect.size(); ++j)
        {
            if (path[i] == stadiumsVect[j].getStadiumName())
            {
                pathStadiums.push_back(stadiumsVect[j]);
            }
        }
    }

    cart.setPath(pathStadiums);
}

vector<Stadium> BackendManager::getCartPath() 
{
    return cart.getPath();
}

int BackendManager::getCartTotalDistance()
{
    return cart.getTotalDistance();
}