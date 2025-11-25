// backendManager.cpp

#include "../../include/models/backendManager.h"

BackendManager::BackendManager() : isAdmin{false}
{
    populateStadiums();
    populateDistances();
    printStadiums();

    adjacencyMatrix.printMatrix();
    // adjacencyMatrix.BFS("State Farm Stadium");

    adjacencyMatrix.Dijkstra("State Farm Stadium");
    adjacencyMatrix.mst("State Farm Stadium");
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
    // AdjacencyMatrix
    adjacencyMatrix.setDistances(databaseManager.getAllDistances());
    adjacencyMatrix.populateVertices(stadiums);

    // AdjacencyList
    
}


void BackendManager::printStadiums()
{
    stadiums.printTable();
}


void BackendManager::addTeam()
{

}

void BackendManager::addSouvenir()
{

}

void BackendManager::removeSouvinir()
{

}

void BackendManager::modifySouvenirPrice()
{

}