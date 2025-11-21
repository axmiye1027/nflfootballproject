// backendManager.cpp

#include "../../include/models/backendManager.h"

BackendManager::BackendManager() : isAdmin{false}
{

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
    vector<StadiumStruct> stadiumsVect = databaseManager.getAllStadiums();

    for (int i = 0; i < stadiumsVect.size(); ++i)
    {
        Stadium stadium;
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

        stadium.setStadiumId(stadiumsVect[i].stadiumId);
        stadium.setTeamName(stadiumsVect[i].teamName);
        stadium.setStadiumName(stadiumsVect[i].stadiumName);
        stadium.setCapacity(stadiumsVect[i].capacity);
        stadium.setLocation(stadiumsVect[i].location);
        stadium.setRoofType(roof);
        stadium.setSurfaceType(stadiumsVect[i].surface);
        stadium.setDateOpened(stadiumsVect[i].yearOpened);
        stadium.setConference(stadiumsVect[i].conference);
        stadium.setDivision(stadiumsVect[i].division);

        stadiums.insert(stadium);

    }


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