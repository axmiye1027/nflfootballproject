#include "backendManager.h"

BackendManager::BackendManager() : isAdmin{false}
{

}

BackendManager::~BackendManager()
{

}

void BackendManager::login(string username, string password)
{
    if(username == "admin" && password == "admin")
    {
        cout << "Login successful." << endl;
        isAdmin = true;
    }
    else
    {
        cout << "Invalid credentials." << endl;
    }
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