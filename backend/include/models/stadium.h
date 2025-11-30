#include "../includes.h"
#include "hash.h"
#include "../database/databaseManager.h"

#ifndef STADIUM_H
#define STADIUM_H

enum RoofType {OPEN, FIXED, RETRACTABLE, NONE};

class Stadium
{
public:
    Stadium(); // Default Constructor
    Stadium(string teamName, string stadiumName, int capacity, string location, RoofType roofType, string surface, // Overloade Const
            int dateOpened, string conference, string division, DoubleHashTable<Souvenir> souvenirList);

    Stadium(const Stadium& other);
    
    Stadium& operator=(const Stadium& other);


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
    void setYearOpened(int yearOpened);
    void setConference(string conference);
    void setDivision(string division);
    void setSouvenirList(DoubleHashTable<Souvenir> souvenirList);
    /* --------------------- */

    /* ------ Getters ------ */
    int      getStadiumId() const;
    string   getTeamName() const;
    string   getStadiumName() const;
    int      getCapacity() const;
    string   getLocation() const;
    RoofType getRoofType() const;
    string   getSurfaceType() const;
    int      getYearOpened() const;
    string   getConference() const;
    string   getDivision() const;
    int      getSouvenirId(string name) const;
    DoubleHashTable<Souvenir> getSouvenirList() const;
    /* --------------------- */

    void addSouvenir(Souvenir souvenir);

    friend std::ostream& operator<<(std::ostream& os, const Stadium& s);

    bool operator==(const string& other) const 
    {
        return stadiumName == other;
    }

    void printSouvenirs() const
    {
        vector<Souvenir> souvenirVec = souvenirList.getValues();

        for(int i = 0; i < souvenirVec.size(); ++i)
        {
            cout << "Name: " << souvenirVec[i].souvenirName << " Price: $" << souvenirVec[i].souvenirPrice << endl;
        }
    }

private:
    int      stadiumId;
    string   teamName;
    string   stadiumName;
    int      capacity;
    string   location; // city, state
    RoofType roofType;
    string   surfaceType;
    int      yearOpened;
    string   conference;
    string   division;

    DoubleHashTable<Souvenir> souvenirList; // If each team/stadium has their own souvenirs
};

#endif