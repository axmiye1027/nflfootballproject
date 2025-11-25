// routes.h

#include "crow.h"
#include "../models/stadium.h"
#include "../models/backendManager.h"

/* ----  Helper Functions ----- */
crow::json::wvalue stadiumToJson(const Stadium& stadium);
crow::json::wvalue stadiumListToJson(const vector<Stadium>& stadiums);
/* ---------------------------- */

void registerRoutes(crow::SimpleApp& app, BackendManager& backend);

