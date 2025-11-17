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

void BackendManager::AddTeam()
{

}

void BackendManager::AddSouvenir()
{

}

void BackendManager::RemoveSouvinir()
{

}

void BackendManager::ModifySouvinirPrice()
{

}