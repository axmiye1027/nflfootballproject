// stadium.cpp

#include "../../include/models/stadium.h"

std::ostream& operator<<(std::ostream& os, const Stadium& s) 
{
    os << s.stadiumName;
    return os;
}

Stadium::Stadium() : teamName(""), stadiumName(""), capacity(0), location(""), roofType(NONE), surfaceType(""), yearOpened(0), conference(""), division(""), souvenirList()
{
}

Stadium::Stadium(string teamName, string stadiumName, int capacity, string location, RoofType roofType, string surface, 
    int yearOpened, string conference, string division, DoubleHashTable<Souvenir> souvenirList)
: teamName(teamName), stadiumName(stadiumName), capacity(capacity), location(location), roofType(roofType), surfaceType(surface), 
    yearOpened(yearOpened), conference(conference), division(division), souvenirList(souvenirList)
{
}

Stadium::Stadium(const Stadium& other)
    : stadiumId(other.stadiumId),
      teamName(other.teamName),
      stadiumName(other.stadiumName),
      capacity(other.capacity),
      location(other.location),
      roofType(other.roofType),
      surfaceType(other.surfaceType),
      yearOpened(other.yearOpened),
      conference(other.conference),
      division(other.division),
      souvenirList(other.souvenirList) {}

Stadium& Stadium::operator=(const Stadium& other)
{
    if (this == &other)
    {
        return *this;
    }

    stadiumId = other.stadiumId;
    teamName = other.teamName;
    stadiumName = other.stadiumName;
    capacity = other.capacity;
    location = other.location;
    roofType = other.roofType;
    surfaceType = other.surfaceType;
    yearOpened = other.yearOpened;
    conference = other.conference;
    division = other.division;
    souvenirList = other.souvenirList;

    return *this;
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
void Stadium::setYearOpened(int yearOpened)          { this->yearOpened     = yearOpened;  }
void Stadium::setConference(string conference)       { this->conference     = conference;  }
void Stadium::setDivision(string division)           { this->division       = division;    }

void Stadium::setSouvenirList(DoubleHashTable<Souvenir> souvenirList) { this->souvenirList = souvenirList; }
/* --------------------- */


int      Stadium::getStadiumId()   { return stadiumId;   }
string   Stadium::getTeamName()    { return teamName;    }
string   Stadium::getStadiumName() { return stadiumName; }
int      Stadium::getCapacity()    { return capacity;    }
string   Stadium::getLocation()    { return location;    }
RoofType Stadium::getRoofType()    { return roofType;    }
string   Stadium::getSurfaceType() { return surfaceType; }
int      Stadium::getYearOpened()  { return yearOpened;  }
string   Stadium::getConference()  { return conference;  }
string   Stadium::getDivision()    { return division;    }
DoubleHashTable<Souvenir> getSouvenirList();

void Stadium::addSouvenir(Souvenir souvenir) { //souvenirList.insert(souvenir); 
    } // needs a key