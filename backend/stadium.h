// stadium.h

#pragma once

#include <iostream>
#include <string>
#include <vector>
using namespace std;

enum RoofType {OPEN, FIXED, RETRACTABLE};

struct Souvenir
{
    int    stadiumId;
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
    Stadium(string teamName, string stadiumName, int capacity, Location location, string roofType, string surface, // Overloaded Constructor
            int dateOpened, string conference, string division, vector<Souvenir> souvenirList);
    Stadium(string teamName, string stadiumName, int capacity, string city, string state, string roofType, string surface, // Overloade Const (location split into 2 strings)
            int dateOpened, string conference, string division, vector<Souvenir> souvenirList);

    ~Stadium(); // Destructor

    /* ------ Setters ------ */
    void setTeamName(string teamName);
    void setStadiumName(string stadiumName);
    void setCapacity(int capacity);
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
    int      stadiumId;
    string   teamName;
    string   stadiumName;
    int      capacity;
    Location location; // could be pair<city, state> or two strings
    RoofType roofType;
    string   surfaceType;
    int      dateOpened;
    string   conference;
    string   division;

    vector<Souvenir> souvenirList; // If each team/stadium has their own souvenirs
};