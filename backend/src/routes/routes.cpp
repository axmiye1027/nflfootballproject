// routes.cpp

#include "includes.h"
#include "routes/routes.h"

crow::json::wvalue stadiumToJson(const Stadium& stadium)
{
    crow::json::wvalue json;
    string roof;
    switch (stadium.getRoofType())
    {
        case OPEN: roof = "Open";
        break;
        
        case FIXED_ROOF: roof = "Fixed";
        break;
        
        case RETRACTABLE: roof = "Retractable";
        break;
    }

    json["stadiumId"]   = stadium.getStadiumId();
    json["teamName"]    = stadium.getTeamName();
    json["stadiumName"] = stadium.getStadiumName();
    json["capacity"]    = stadium.getCapacity();
    json["location"]    = stadium.getLocation();
    json["roofType"]    = roof;
    json["surfaceType"] = stadium.getSurfaceType();
    json["yearOpened"]  = stadium.getYearOpened();
    json["conference"]  = stadium.getConference();
    json["division"]    = stadium.getDivision();

    // need method for souvenirlist

    return json;
}


crow::json::wvalue stadiumListToJson(const vector<Stadium>& stadiums)
{
    crow::json::wvalue json;
    
    auto& arr = json["stadiums"];
    arr = crow::json::wvalue::list(stadiums.size());


    for (int i = 0; i < stadiums.size(); ++i)
    {
        arr[i] = stadiumToJson(stadiums[i]);
    }

    return json;
}

void registerRoutes(crow::SimpleApp& app, BackendManager& backend)
{
    // respond to OPTIONS preflight
    CROW_ROUTE(app, "/stadiums").methods(crow::HTTPMethod::OPTIONS)
    ([]() 
    {
        crow::response r;
        r.add_header("Access-Control-Allow-Origin", "*");
        r.add_header("Access-Control-Allow-Methods", "GET, OPTIONS");
        r.add_header("Access-Control-Allow-Headers", "*");
        r.code = 200;
        return r;
    });

    CROW_ROUTE(app, "/stadiums").methods(crow::HTTPMethod::GET)
    ([&backend] 
    {
        auto stadiums = backend.getStadiumsAsVector();
        auto json     = stadiumListToJson(stadiums);

        crow::response r(json.dump());
        r.add_header("Access-Control-Allow-Origin", "*");
        r.add_header("Content-Type", "application/json");
        return r;
    });


}