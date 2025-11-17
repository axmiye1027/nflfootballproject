#include "../includes.h"
#include "../../hash.h"

#ifndef STADIUM_H
#define STADIUM_H

enum RoofType {OPEN, FIXED_ROOF, RETRACTABLE};

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
        
        return *this;
    }
};


class Stadium
{
public:
    Stadium(); // Default Constructor
    Stadium(string teamName, string stadiumName, int capacity, Location location, RoofType roofType, string surface, // Overloaded Constructor
            int dateOpened, string conference, string division, DoubleHashTable<Souvenir> souvenirList);
    Stadium(string teamName, string stadiumName, int capacity, string city, string state, RoofType roofType, string surface, // Overloade Const (location split into 2 strings)
            int dateOpened, string conference, string division, DoubleHashTable<Souvenir> souvenirList);

    ~Stadium(); // Destructor

    /* ------ Setters ------ */
    void setTeamName(string teamName);
    void setStadiumName(string stadiumName);
    void setCapacity(int capacity);
    void setLocation(string city, string location);
    void setLocation(Location location);
    void setRoofType(RoofType roofType);
    void setSurfaceType(string surfaceType);
    void setDateOpened(int dateOpened);
    void setConference(string conference);
    void setDivision(string division);
    void setSouvenirList(DoubleHashTable<Souvenir> souvenirList);
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

    DoubleHashTable<Souvenir> souvenirList; // If each team/stadium has their own souvenirs
};

#endif