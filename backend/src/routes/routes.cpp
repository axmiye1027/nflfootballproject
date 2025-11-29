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

void registerRoutes(crow::SimpleApp& app, BackendManager& backend)
{
    // respond to OPTIONS preflight
    CROW_ROUTE(app, "/stadiums").methods(crow::HTTPMethod::OPTIONS)
    ([]() 
    {
        crow::response r;
        r.add_header("Access-Control-Allow-Origin", "*");
        r.add_header("Access-Control-Allow-Headers", "*");
        r.add_header("Access-Control-Allow-Methods", "GET");
        r.add_header("Content-Type", "application/json");
        r.code = 200;
        return r;
    });

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

        auto json = stadiumListToJson(stadiums);
        cout << json.dump() << std::endl;

        crow::response r(json.dump());
        r.add_header("Access-Control-Allow-Origin", "*");
        r.add_header("Access-Control-Allow-Headers", "*");
        r.add_header("Access-Control-Allow-Methods", "GET");
        r.add_header("Content-Type", "application/json");
        return r;
    });

    // CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::OPTIONS)
    // ([]() 
    // {
    //     crow::response r;
    //     r.add_header("Access-Control-Allow-Origin", "*");
    //     r.add_header("Access-Control-Allow-Methods", "POST, OPTIONS");
    //     r.add_header("Access-Control-Allow-Headers", "*");
    //     r.code = 200;
    //     return r;
    // });

    // CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::POST)
    // ([&backend](const crow::request& req) 
    // {
    //     auto body = crow::json::load(req.body);
    //     if (!body) 
    //     {
    //         crow::response r(R"({ "success": false, "message": "Invalid JSON" })");
    //         r.code = 400;
    //         r.add_header("Access-Control-Allow-Origin", "*");
    //         r.add_header("Content-Type", "application/json");
    //         return r;
    //     }

    //     string username = body["username"].s();
    //     string password = body["password"].s();

    //     bool valid = backend.login(username, password);

    //     crow::json::wvalue res;
    //     res["success"] = valid;
    //     if (!valid) {
    //         res["message"] = "Invalid username or password";
    //     }

    //     crow::response r(res);
    //     r.add_header("Access-Control-Allow-Origin", "*");
    //     r.add_header("Content-Type", "application/json");
    //     return r;
    // });


}