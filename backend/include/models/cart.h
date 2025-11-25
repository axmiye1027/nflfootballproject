// cart.h

#pragma once

#include "../includes.h"
#include "stadium.h"

class Cart
{
public:
    Cart();
    ~Cart();

    void clearCart();
    void clearPath();
    void clearPurchases();

    void setPath(vector<StadiumNode> path);
    void setPurchases(vector<Souvenir> purchases);

    void addPurchase(Souvenir souvenir);


private:

    vector<StadiumNode> outgoingPath;
    vector<Souvenir>    purchases;

    int totalDistance;
    int totalSpent;

};