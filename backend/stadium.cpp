// stadium.cpp

#include "stadium.h"


Stadium::Stadium()
{
}


Stadium::Stadium(string teamName, string stadiumName, int capacity, Location location, string roofType, string surface, 
    int dateOpened, string conference, string division, vector<Souvenir> souvenirList)
: teamName(teamName), stadiumName(stadiumName), capacity(capacity), location(location), roofType(roofType), surfaceType(surface), 
    dateOpened(dateOpened), conference(conference), division(division), souvenirList(souvenirList)
{
}


Stadium::Stadium(string teamName, string stadiumName, int capacity, string city, string state, string roofType, string surface, 
    int dateOpened, string conference, string division, vector<Souvenir> souvenirList)
: teamName(teamName), stadiumName(stadiumName), capacity(capacity), roofType(roofType), surfaceType(surface), 
    dateOpened(dateOpened), conference(conference), division(division), souvenirList(souvenirList)
{
    setLocation(city, state);
}


Stadium::~Stadium()
{

}


/* ------ Setters ------ */
void Stadium::setTeamName(string teamName)           { this->teamName       = teamName;    }
void Stadium::setStadiumName(string stadiumName)     { this->stadiumName    = stadiumName; }
void Stadium::setCapacity(int capacity)              { this->capacity       = capacity;    }
void Stadium::setLocation(string city, string state) { this->location.city  = city; 
                                                       this->location.state = state;       }
void Stadium::setLocation(Location location)         { this->location       = location;    }
void Stadium::setRoofType(string roofType)           { this->roofType       = roofType;    }
void Stadium::setSurfaceType(string surfaceType)     { this->surfaceType    = surfaceType; }
void Stadium::setDateOpened(int dateOpened)          { this->dateOpened     = dateOpened;  }
void Stadium::setConference(string conference)       { this->conference     = conference;  }
void Stadium::setDivision(string division)           { this->division       = division;    }

void Stadium::setSouvenirList(vector<Souvenir> souvenirList) { this->souvenirList = souvenirList; }
/* --------------------- */

void Stadium::addSouvenir(Souvenir souvenir) { souvenirList.push_back(souvenir); }