// backendManager.cpp

#include "../../include/models/backendManager.h"

BackendManager::BackendManager() : isAdmin{false}
{
    populateStadiums();
    populateDistances();
    printStadiums();

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


void BackendManager::printStadiums()
{
    stadiums.printTable();
}


void BackendManager::addStadium(string teamName, string stadiumName, int capacity, string location, RoofType roofType, string surface,
            int dateOpened, string conference, string division, vector<Souvenir> souvenirList)
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

    databaseManager.addStadium(teamName, stadiumName, capacity, location, roof, surface, dateOpened, conference, division, souvenirList);

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

vector<Stadium> BackendManager::getStadiumsByDivision(string division)
{
    vector<Stadium> stadiums = getStadiumsAsVector();
    vector<Stadium> divisions;

    for(int i = 0; i < stadiums.size(); ++i)
    {
        if(stadiums[i].getDivision() == division)
        {
            divisions.push_back(stadiums[i]);
        }
    }

    return divisions;
}