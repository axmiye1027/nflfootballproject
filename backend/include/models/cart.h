// cart.h

#pragma once

#include "../database/databaseManager.h"
#include "../models/stadium.h"

class Cart
{
public:
    Cart();
    ~Cart();

    void clearCart();
    void clearPath();
    void clearPurchases();

    /* ----- GETTERS ----- */
    vector<Stadium> getPath();
    vector<Souvenir> getPurchases();
    int getTotalDistance();
    int getTotalSpent();
    /* ------------------- */

    void setPath(vector<Stadium> path);
    void setPurchases(vector<Souvenir> purchases);

    void addPurchase(Souvenir souvenir);


private:

    vector<Stadium> outgoingPath;
    vector<Souvenir> purchases;

    int totalDistance;
    int totalSpent;

};