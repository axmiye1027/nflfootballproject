// stadium.h

#pragma once

#include <iostream>
#include <string>
#include <vector>
using namespace std;


struct Souvenir
{
    string name;
    double price;
};

struct Location
{
    string city;
    string state;

    Location& operator=(const Location& location)
    {
        this->city  = location.city;
        this->state = location.state;
    }
};


class Stadium
{
public:
    Stadium(); // Default Constructor
    Stadium(Location location, string roofType, string surface, int dateOpened, string conference, string division, vector<Souvenir> souvenirList); // Overloaded Constructor
    Stadium(string city, string state, string roofType, string surface, int dateOpened, string conference, string division, vector<Souvenir> souvenirList);

    ~Stadium(); // Destructor

    /* ------ Setters ------ */
    void setLocation(string city, string location);
    void setLocation(Location location);
    void setRoofType(string roofType);
    void setSurfaceType(string surfaceType);
    void setDateOpened(int dateOpened);
    void setConference(string conference);
    void setDivision(string division);
    void setSouvenirList(vector<Souvenir> souvenirList);
    /* --------------------- */

    void addSouvenir(Souvenir souvenir);



private:
    Location location; // could be pair<city, state> or two strings
    string   roofType; // could be enum
    string   surfaceType;
    int      dateOpened;
    string   conference;
    string   division;

    vector<Souvenir> souvenirList; // If each team/stadium has their own souvenirs
};