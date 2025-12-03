// routes.h

#include "crow.h"
#include "crow/middlewares/cors.h"

#include "../models/stadium.h"
#include "../models/backendManager.h"
#include "includes.h"

/* ----  Helper Functions ----- */
crow::json::wvalue stadiumToJson(const Stadium& stadium);
crow::json::wvalue souvenirToJson(const Souvenir& s);
crow::json::wvalue stadiumListToJson(const vector<Stadium>& stadiums);
/* ---------------------------- */

void registerRoutes(crow::App<crow::CORSHandler>& app, BackendManager& backend);

