// cart.h

#pragma once

#include "../includes.h"
#include "stadium.h"

struct StadiumNode
{
    Stadium stadium;
    int     distanceFromLast;
};

class Cart
{
public:
    Cart();
    ~Cart();

    void clearCart();
    void clearPath();
    void clearPurchases();

    /* ----- GETTERS ----- */
    vector<StadiumNode> getPath();
    vector<Souvenir>    getPurchases();
    int getTotalDistance();
    int getTotalSpent();
    /* ------------------- */

    void setPath(vector<StadiumNode> path);
    void setPurchases(vector<Souvenir> purchases);

    void addPurchase(Souvenir souvenir);


private:

    vector<StadiumNode> outgoingPath;
    vector<Souvenir>    purchases;

    int totalDistance;
    int totalSpent;

};