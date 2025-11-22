#include "../includes.h"
#include "hash.h"
#include "../database/databaseManager.h"

#ifndef STADIUM_H
#define STADIUM_H

enum RoofType {OPEN, FIXED_ROOF, RETRACTABLE, NONE};

class Stadium
{
public:
    Stadium(); // Default Constructor
    Stadium(string teamName, string stadiumName, int capacity, string location, RoofType roofType, string surface, // Overloade Const
            int dateOpened, string conference, string division, DoubleHashTable<Souvenir> souvenirList);

    ~Stadium(); // Destructor

    /* ------ Setters ------ */
    void setStadiumId(int id);
    void setTeamName(string teamName);
    void setStadiumName(string stadiumName);
    void setCapacity(int capacity);
    void setLocation(string city, string location);
    void setLocation(string location);
    void setRoofType(RoofType roofType);
    void setSurfaceType(string surfaceType);
    void setDateOpened(int dateOpened);
    void setConference(string conference);
    void setDivision(string division);
    void setSouvenirList(DoubleHashTable<Souvenir> souvenirList);
    /* --------------------- */

    void addSouvenir(Souvenir souvenir);

    friend std::ostream& operator<<(std::ostream& os, const Stadium& s);

    bool operator==(const string& other) const 
    {
        return stadiumName == other;
    }

private:
    int      stadiumId;
    string   teamName;
    string   stadiumName;
    int      capacity;
    string   location; // city, state
    RoofType roofType;
    string   surfaceType;
    int      dateOpened;
    string   conference;
    string   division;

    DoubleHashTable<Souvenir> souvenirList; // If each team/stadium has their own souvenirs
};

#endif