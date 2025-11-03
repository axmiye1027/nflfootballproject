#include <iostream>
#include <vector>

using namespace std;

class BackendManager
{
public:
    BackendManager();
    ~BackendManager();

    void login(string username, string password);
    
    void AddTeam();
    void AddSouvenir();
    void RemoveSouvinir();
    void ModifySouvinirPrice();

private:


    bool isAdmin;
};