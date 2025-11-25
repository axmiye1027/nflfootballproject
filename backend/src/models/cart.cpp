// cart.cpp

#include "cart.h"

Cart::Cart() {}
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


void Cart::setPath(vector<StadiumNode> path)
{

}


void Cart::setPurchases(vector<Souvenir> purchases)
{

}


void Cart::addPurchase(Souvenir souvenir)
{

}