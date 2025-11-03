// stadium.cpp

#include "stadium.h"


Stadium::Stadium()
{

}


Stadium::Stadium(Location location, string roofType, string surface, int dateOpened, string conference, string division, vector<Souvenir> souvenirList)
: location(location), roofType(roofType), surfaceType(surface), dateOpened(dateOpened), conference(conference), division(division), souvenirList(souvenirList)
{
}


Stadium::Stadium(string city, string state, string roofType, string surface, int dateOpened, string conference, string division, vector<Souvenir> souvenirList)
: roofType(roofType), surfaceType(surface), dateOpened(dateOpened), conference(conference), division(division), souvenirList(souvenirList)
{
    setLocation(city, state);
}


Stadium::~Stadium()
{

}


/* ------ Setters ------ */
void Stadium::setLocation(string city, string state) { location.city = city; location.state = state; }
void Stadium::setLocation(Location location)         { this->location    = location;    }
void Stadium::setRoofType(string roofType)           { this->roofType    = roofType;    }
void Stadium::setSurfaceType(string surfaceType)     { this->surfaceType = surfaceType; }
void Stadium::setDateOpened(int dateOpened)          { this->dateOpened  = dateOpened;  }
void Stadium::setConference(string conference)       { this->conference  = conference;  }
void Stadium::setDivision(string division)           { this->division    = division;    }

void Stadium::setSouvenirList(vector<Souvenir> souvenirList) { this->souvenirList = souvenirList; }
/* --------------------- */

void Stadium::addSouvenir(Souvenir souvenir) { souvenirList.push_back(souvenir); }