#include <iostream>
#include <vector>

using namespace std;

class BackendManager
{
public:
    BackendManager();
    ~BackendManager();

    void login(string username, string password);

private:


    bool isAdmin;
};