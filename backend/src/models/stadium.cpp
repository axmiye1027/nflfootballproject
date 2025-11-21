// stadium.cpp

#include "../../include/models/stadium.h"


Stadium::Stadium()
{
}


Stadium::Stadium(string teamName, string stadiumName, int capacity, string location, RoofType roofType, string surface, 
    int dateOpened, string conference, string division, DoubleHashTable<Souvenir> souvenirList)
: teamName(teamName), stadiumName(stadiumName), capacity(capacity), location(location), roofType(roofType), surfaceType(surface), 
    dateOpened(dateOpened), conference(conference), division(division), souvenirList(souvenirList)
{
}


Stadium::~Stadium()
{

}


/* ------ Setters ------ */
void Stadium::setStadiumId(int id)                   { this->stadiumId      = id;          }
void Stadium::setTeamName(string teamName)           { this->teamName       = teamName;    }
void Stadium::setStadiumName(string stadiumName)     { this->stadiumName    = stadiumName; }
void Stadium::setCapacity(int capacity)              { this->capacity       = capacity;    }
void Stadium::setLocation(string location)           { this->location       = location;    }
void Stadium::setRoofType(RoofType roofType)         { this->roofType       = roofType;    }
void Stadium::setSurfaceType(string surfaceType)     { this->surfaceType    = surfaceType; }
void Stadium::setDateOpened(int dateOpened)          { this->dateOpened     = dateOpened;  }
void Stadium::setConference(string conference)       { this->conference     = conference;  }
void Stadium::setDivision(string division)           { this->division       = division;    }

void Stadium::setSouvenirList(DoubleHashTable<Souvenir> souvenirList) { this->souvenirList = souvenirList; }
/* --------------------- */

void Stadium::addSouvenir(Souvenir souvenir) { //souvenirList.insert(souvenir); 
    } // needs a key