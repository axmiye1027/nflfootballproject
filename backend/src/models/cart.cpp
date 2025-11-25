// cart.cpp

#include "cart.h"

Cart::Cart() : totalDistance(0), totalSpent(0) {}
Cart::~Cart() {}


void Cart::clearCart() 
{ 
    clearPath();
    clearPurchases();

    totalDistance = 0;
    totalSpent    = 0;
}


void Cart::clearPath()
{
    outgoingPath.clear();
}


void Cart::clearPurchases()
{
    purchases.clear();
}


/* ----- GETTERS ----- */
vector<StadiumNode> Cart::getPath()      { return outgoingPath;  }
vector<Souvenir>    Cart::getPurchases() { return purchases;     }
int Cart::getTotalDistance()             { return totalDistance; }
int Cart::getTotalSpent()                { return totalSpent;    }
/* ------------------- */


void Cart::setPath(vector<StadiumNode> path)
{
    outgoingPath = path;
}


void Cart::setPurchases(vector<Souvenir> purchases)
{
    this->purchases = purchases;
}


void Cart::addPurchase(Souvenir souvenir)
{
    purchases.push_back(souvenir);
}