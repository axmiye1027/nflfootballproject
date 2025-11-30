// backendManager.cpp

#include "../../include/models/backendManager.h"

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

BackendManager::BackendManager() : isAdmin{false}
{
    populateStadiums();
    // populateDistances();
    // printStadiums();

    //adjacencyMatrix.printMatrix();
    //adjacencyMatrix.bfs("State Farm Stadium");

    //adjacencyMatrix.Dijkstra("State Farm Stadium");
    //adjacencyMatrix.mst("State Farm Stadium");

    //adjacencyList.dfs("State Farm Stadium");
}

BackendManager::~BackendManager()
{

}


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
    try {
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
vector<Stadium> sortStadiums(const vector<Stadium>& stadiumsVect, Compare compare)
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
    transform(roofType.begin(), roofType.end(), roofType.begin(), ::toupper);

    vector<Stadium> roofTypes;

    for(int i = 0; i < stadiumsVect.size(); ++i)
    {
        string stadiumRoof;

        switch(stadiumsVect[i].getRoofType())
        {
            case OPEN:
                stadiumRoof = "OPEN";
            break;

            case FIXED:
                stadiumRoof = "FIXED ROOF";
            break;

            case RETRACTABLE:
                stadiumRoof = "RETRACTABLE";
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

Stadium BackendManager::getStadiumByName(const vector<Stadium>& stadiumsVect,string stadiumName)
{
    for(int i = 0; i < stadiumsVect.size(); ++i)
    {
        if(stadiumsVect[i].getStadiumName == stadiumName)
        {
            return stadiumsVect[i];
        }
    }

    cout << "Stadium Not Found." << endl;
}

vector<Stadium> BackendManager::getStadiumsByDivision(const vector<Stadium>& stadiumsVect,string division)
{
    transform(division.begin(), division.end(), division.begin(), ::toupper);

    vector<Stadium> divisions;

    for(int i = 0; i < stadiumsVect.size(); ++i)
    {
        if(stadiumsVect[i].getDivision().substr(0, 3) == division)
        {
            divisions.push_back(stadiumsVect[i]);
        }
    }

    return divisions;
}

vector<Stadium> BackendManager::getStadiumsByConference(const vector<Stadium>& stadiumsVect,string conference)
{
    transform(conference.begin(), conference.end(), conference.begin(), ::toupper);

    vector<Stadium> conferences;

    for(int i = 0; i < stadiumsVect.size(); ++i)
    {
        if(stadiumsVect[i].getConference().substr(0, 8) == conference)
        {
            conferences.push_back(stadiumsVect[i]);
        }
    }

    return conferences;
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
