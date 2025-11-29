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

    // initialize as a JSON array
    const vector<Souvenir>& souvenirs = stadium.getSouvenirList().getValues();

    json["souvenirs"] = crow::json::wvalue(crow::json::type::List);
    auto& jsonArr = json["souvenirs"];

    for (int i = 0; i < souvenirs.size(); ++i)
    {
        jsonArr[i] = souvenirToJson(souvenirs[i]);
    }

    return json;
}


crow::json::wvalue souvenirToJson(const Souvenir& s)
{
    crow::json::wvalue json;
    json["souvenirId"]    = s.souvenirId;
    json["stadiumId"]     = s.stadiumId;
    json["souvenirName"]  = s.souvenirName;
    json["souvenirPrice"] = s.souvenirPrice;
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

void registerRoutes(crow::App<crow::CORSHandler>& app, BackendManager& backend)
{

    CROW_ROUTE(app, "/stadiums").methods(crow::HTTPMethod::GET)
    ([&backend](const crow::request& req)
    {
        string division = req.url_params.get("division")
                            ? req.url_params.get("division")
                            : "All Teams";

        string search = req.url_params.get("search") ? req.url_params.get("search") : "";

        vector<Stadium> stadiums;

        if (division == "All Teams")
        {
            stadiums = backend.getStadiumsAsVector();
        }
        else
        {
            stadiums = backend.getStadiumsByDivision(division);
        }

        if (!search.empty())
        {
            stadiums = backend.filterStadiums(stadiums, search);
        }

        crow::json::wvalue json = stadiumListToJson(stadiums);

        return crow::response{ json.dump() };
    });

    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::POST)
    ([&backend](const crow::request& req) 
    {
        auto body = crow::json::load(req.body);
        if (!body)
        {
            crow::json::wvalue error;
            error["success"] = false;
            error["message"] = "Invalid JSON";
            return crow::response(400, error.dump());
        }

        string username = body["username"].s();
        string password = body["password"].s();

        bool valid = backend.login(username, password);

        crow::json::wvalue res;
        res["success"] = valid;
        if (!valid)
            res["message"] = "Invalid username or password";

        return crow::response(res.dump());
    });


}