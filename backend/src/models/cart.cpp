// cart.cpp

#include "models/cart.h"

Cart::Cart() : totalDistance(0), totalSpent(0) {}
Cart::~Cart() {}


void Cart::clearCart() 
{ 
    clearPath();
    clearPurchases();
}


void Cart::clearPath()
{
    outgoingPath.clear();
    totalDistance = 0;
}


void Cart::clearPurchases()
{
    purchases.clear();
    totalSpent = 0;
}


/* ----- GETTERS ----- */
vector<Stadium> Cart::getPath()      { return outgoingPath;  }
vector<Souvenir> Cart::getPurchases() { return purchases;     }
int Cart::getTotalDistance()          { return totalDistance; }
int Cart::getTotalSpent()             { return totalSpent;    }
/* ------------------- */


void Cart::setPath(vector<Stadium> path)
{
    this->outgoingPath = path;
}


void Cart::setPurchases(vector<Souvenir> purchases)
{
    this->purchases = purchases;
}


void Cart::addPurchase(Souvenir souvenir)
{
    purchases.push_back(souvenir);
}