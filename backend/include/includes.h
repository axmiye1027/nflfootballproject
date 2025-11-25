#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>    // sort
#include <iomanip>      // setw, left
#include <unordered_map>
#include <set>
#include <deque>
#include <limits>

using namespace std;

/* ------- DATABASE MANAGER ------- */
struct Distance 
{
    int    id;
    string locationA;
    string locationB;
    int    distanceKm;
};

struct StadiumStruct
{
    int    stadiumId;
    string teamName;
    string stadiumName;
    int    capacity;
    string location;
    string roofType;
    string surface;
    int    yearOpened;
    string conference;
    string division;
};

struct Souvenir
{
    int    stadiumId;
    int    souvenirId;
    string souvenirName;
    double souvenirPrice;

    friend ostream& operator<<(ostream& os, const Souvenir& souvenir) 
    {
        os << souvenir.souvenirName << " : " << souvenir.souvenirPrice;
        return os;
    }
};
/* ------------------------- */


/* ----- CART ----- */
struct StadiumNode
{
    Stadium stadium;
    int     distanceFromLast;
};
/* ------------------------- */
