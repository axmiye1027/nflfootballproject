/**
 * @file routes.cpp
 * @brief file containing all the routes
 */

//#include "includes.h"
#include "routes/routes.h"

/**
 * @brief gets stadium information and puts it in a json file
 * @param stadium current stadium we are storing the data of
 */
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

/**
 * @brief gets the souvenirs and converts it to a json file
 * @param s souveniers 
 */
crow::json::wvalue souvenirToJson(const Souvenir& s)
{
    crow::json::wvalue json;
    json["souvenirId"]    = s.souvenirId;
    json["stadiumId"]     = s.stadiumId;
    json["souvenirName"]  = s.souvenirName;
    json["souvenirPrice"] = s.souvenirPrice;
    return json;
}

crow::json::wvalue stringToJson(const string& s)
{
    crow::json::wvalue json;
    json["stadiumName"] = s;
    return json;
}

crow::json::wvalue stringListToJson(const vector<string>& vec)
{
    crow::json::wvalue json;

    json["stadiums"] = crow::json::wvalue(crow::json::type::List);
    auto& jsonArr = json["stadiums"];

    for (int i = 0; i < vec.size(); ++i)
    {
        jsonArr[i] = stringToJson(vec[i]);
    }

    return json;
}


/**
 * @brief converts the stadiums in a json and puts it into an array
 * @param stadiums stadiums inside a json 
 */
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

/**
 * @brief searching
 */
void registerRoutes(crow::App<crow::CORSHandler>& app, BackendManager& backend)
{

    CROW_ROUTE(app, "/stadiums").methods(crow::HTTPMethod::GET)
    ([&backend](const crow::request& req)
    {
        const string ALL_TEAMS = "All Teams";
        
        
        //NEW - for dropdown
        string sortBy = req.url_params.get("sortBy") ? req.url_params.get("sortBy") : ""; 
        
        
        // SORT
        string teamName    = req.url_params.get("teamName")    ? req.url_params.get("teamName")    : ALL_TEAMS;
        string stadiumName = req.url_params.get("stadiumName") ? req.url_params.get("stadiumName") : ALL_TEAMS;
        string yearOpened  = req.url_params.get("yearOpened")  ? req.url_params.get("yearOpened")  : ALL_TEAMS;
        string capacity    = req.url_params.get("capacity")    ? req.url_params.get("capacity")    : ALL_TEAMS;

        // FILTER
        string conference  = req.url_params.get("conference")  ? req.url_params.get("conference")  : ALL_TEAMS;
        string division    = req.url_params.get("divisions")   ? req.url_params.get("divisions")   : ALL_TEAMS;
        string roofTypes   = req.url_params.get("roofTypes")   ? req.url_params.get("roofTypes")   : ALL_TEAMS; //changed roofType => roofTypes
        string surface     = req.url_params.get("surfaces")    ? req.url_params.get("surfaces")    : ALL_TEAMS;
        string roofType    = req.url_params.get("roofTypes")   ? req.url_params.get("roofTypes")   : ALL_TEAMS;

        // SEARCH
        string search   = req.url_params.get("search") ? req.url_params.get("search") : "";

        vector<Stadium> stadiums = backend.getStadiumsAsVector();

        if (conference != ALL_TEAMS)
        {
            stadiums = backend.getStadiumsByConference(stadiums, conference);
        }

        if (division != ALL_TEAMS)
        {
            stadiums = backend.getStadiumsByDivision(stadiums, division);
        }

        if (roofTypes != ALL_TEAMS)
        {
            stadiums = backend.getStadiumsByRoofType(stadiums, roofTypes);
        }

        if (surface != ALL_TEAMS)
        {
            stadiums = backend.getStadiumsBySurface(stadiums, surface);
        }

        if (!search.empty())
        {
            stadiums = backend.filterStadiums(stadiums, search);
        }

        //NEW - for dropdown
        if (teamName != ALL_TEAMS)
            stadiums = backend.getStadiumsByTeamName(stadiums, teamName);

        if (stadiumName != ALL_TEAMS)
            stadiums = backend.getStadiumsByStadiumName(stadiums, stadiumName);

        //NEW - for dropdown
        if (sortBy == "teamName")
            stadiums = backend.sortStadiumsByTeam(stadiums, "");
        else if (sortBy == "stadiumName")
            stadiums = backend.sortStadiumsByStadiumName(stadiums);
        else if (sortBy == "capacity")
            stadiums = backend.sortStadiumsByCapacity(stadiums);
        else if (sortBy == "yearOpened")
            stadiums = backend.sortStadiumsByDateOpened(stadiums);

        crow::json::wvalue json = stadiumListToJson(stadiums);

        return crow::response{ json.dump() };
    });


    // NEW ROUTE - get count of stadiums with a specific roof type
    CROW_ROUTE(app, "/stadiums/count").methods(crow::HTTPMethod::GET)
    ([&backend](const crow::request& req) {
        // MATCH frontend 'roofTypes' parameter
        string roofType = req.url_params.get("roofTypes") ? req.url_params.get("roofTypes") : "";

        vector<Stadium> stadiums = backend.getStadiumsAsVector();

        if (!roofType.empty()) {
            stadiums = backend.getStadiumsByRoofType(stadiums, roofType);
        }

        crow::json::wvalue res;
        res["count"] = static_cast<int>(stadiums.size());
        return crow::response(res.dump());
    });


    /**
     * @brief gets user login credentials 
     */
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

    /**
     * @brief updates stadium information
     */
    CROW_ROUTE(app, "/updateStadium").methods(crow::HTTPMethod::POST)
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

        int    stadiumId   = body["stadiumId"].i();
        string teamName    = body["teamName"].s();
        string stadiumName = body["stadiumName"].s();
        int    capacity    = body["capacity"].i();
        string location    = body["location"].s();
        string roofType    = body["roofType"].s();
        string surfaceType = body["surfaceType"].s();
        int    yearOpened  = body["yearOpened"].i();
        string conference  = body["conference"].s();
        string division    = body["division"].s();

        bool success = backend.updateStadium(stadiumId, teamName, stadiumName, capacity, location, roofType, surfaceType, yearOpened, conference, division);

        crow::json::wvalue res;
        res["success"] = success;
        if (!success) res["message"] = "Failed to update stadium";

        return crow::response(res);
    });

    /* ------------------------------- TRIPS -------------------------------*/
    CROW_ROUTE(app, "/bfsTrip").methods(crow::HTTPMethod::POST)
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

        string bfsStadium = body["bfsStadium"].s();

        if (bfsStadium.empty())
        {
            crow::json::wvalue error;
            error["success"] = false;
            error["message"] = "At least one city must be provided";
            return crow::response(400, error.dump());
        }

        int bfsResult = 0;
        
        if (!bfsStadium.empty())
            bfsResult = backend.calculateBFS(bfsStadium);

        crow::json::wvalue res;
        res["success"]       = true;
        res["totalDistance"] = bfsResult;
        res["stadiums"]      = bfsStadium;

        return crow::response(res.dump());
    });


    CROW_ROUTE(app, "/mstTrip").methods(crow::HTTPMethod::POST)
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

        string mstStadium = body["mstStadium"].s();

        if (mstStadium.empty())
        {
            crow::json::wvalue error;
            error["success"] = false;
            error["message"] = "At least one city must be provided";
            return crow::response(400, error.dump());
        }

        int mstResult = 0;
        
        if (!mstStadium.empty())
            mstResult = backend.calculateMST(mstStadium);

        crow::json::wvalue res;
        res["success"]       = true;
        res["totalDistance"] = mstResult;
        res["stadiums"]      = mstStadium;
        return crow::response(res.dump());
    });


    CROW_ROUTE(app, "/dijkstraTrip").methods(crow::HTTPMethod::POST)
    ([&backend](const crow::request& req) 
    {
        std::cout << "Raw request body:\n" << req.body << "\n"; // <-- ADD THIS

        auto body = crow::json::load(req.body);
        if (!body)
        {
            crow::json::wvalue error;
            error["success"] = false;
            error["message"] = "Invalid JSON";
            return crow::response(400, error.dump());
        }

        auto startingStadiumJson = body["startingStadium"];
        auto endingStadiumJson   = body["endingStadium"];

        string startingName = startingStadiumJson["stadiumName"].s();
        string endingName   = endingStadiumJson["stadiumName"].s();

        PathReturn path = backend.calculateDijkstra(startingName, endingName);
        vector<string> stringPath = path.path;

        cout << "[DEBUG] Path size: " << stringPath.size() << endl;
        for (size_t i = 0; i < stringPath.size(); ++i)
        {
            if (stringPath.empty()) cout << "[DEBUG] path[" << i << "] is empty\n";
            else cout << "[DEBUG] path[" << i << "]: " << stringPath[i] << endl;
        }

        crow::json::wvalue res;
        res["success"] = true;
        res["totalDistance"] = path.distanceTraveled; 
        res["stadiums"] = crow::json::wvalue(crow::json::type::List);

        auto& arr = res["stadiums"];
        for (size_t i = 0; i < path.path.size(); ++i)
        {
            arr[i] = path.path[i];
        }

        return crow::response(res.dump());
    });


    CROW_ROUTE(app, "/dfsTrip").methods(crow::HTTPMethod::POST)
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

        string startingStadium = body["dfsStadium"].s();

        PathReturn path = backend.calculateDFS(startingStadium);
 
        crow::json::wvalue res;
        res["success"]       = true;
        res["totalDistance"] = path.distanceTraveled;
        res["stadiums"] = crow::json::wvalue(crow::json::type::List);

        auto& arr = res["stadiums"];
        for (size_t i = 0; i < path.path.size(); ++i)
        {
            arr[i] = path.path[i];
        }

        return crow::response(res.dump());

    });


    CROW_ROUTE(app, "/customTrip").methods(crow::HTTPMethod::POST)
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

        vector<string> stadiumNames;

        // Extract stadiums array
        const auto& arr = body["stadiums"];
        if (arr.t() != crow::json::type::List) 
        {
            return crow::response(400, "stadiums must be an array");
        }

        for (const auto& item : arr) 
        {
            if (item.t() == crow::json::type::Object) 
            {
                string name = item["stadiumName"].s();
                stadiumNames.push_back(name);
            }
        }

        PathReturn path = backend.calculateCustomTrip(stadiumNames);

        crow::json::wvalue res;
        res["success"] = true;
        res["totalDistance"] = path.distanceTraveled; 
        res["stadiums"] = crow::json::wvalue(crow::json::type::List);

        auto& stadiumList = res["stadiums"];

        for (size_t i = 0; i < path.path.size(); ++i)
        {
            stadiumList[i] = crow::json::wvalue();   // allocate an empty JSON entry
            stadiumList[i] = path.path[i];           // now safe to assign
        }

        return crow::response(res.dump());
    });


    CROW_ROUTE(app, "/recursiveTrip").methods(crow::HTTPMethod::POST)
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

        vector<string> stadiumNames;

        // Extract stadiums array
        const auto& arr = body["stadiums"];
        if (arr.t() != crow::json::type::List) 
        {
            return crow::response(400, "stadiums must be an array");
        }

        for (const auto& item : arr) 
        {
            if (item.t() == crow::json::type::Object) 
            {
                string name = item["stadiumName"].s();
                stadiumNames.push_back(name);
            }
        }

        PathReturn path = backend.calculateRecursiveTrip(stadiumNames);

        crow::json::wvalue res;
        res["success"] = true;
        res["totalDistance"] = path.distanceTraveled; 
        res["stadiums"] = crow::json::wvalue(crow::json::type::List);

        auto& stadiumList = res["stadiums"];

        for (size_t i = 0; i < path.path.size(); ++i)
        {
            stadiumList[i] = crow::json::wvalue();   // allocate an empty JSON entry
            stadiumList[i] = path.path[i];           // now safe to assign
        }

        return crow::response(res.dump());
    });


    CROW_ROUTE(app, "/souvenirs").methods(crow::HTTPMethod::POST)
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

        vector<string> stadiumNames;
        if (body.has("stadiums") && body["stadiums"].t() == crow::json::type::List)
        {
            for (auto& s : body["stadiums"])
            {
                stadiumNames.push_back(s.s());
                cout << "stadiumNames.push_back(s.s()): " << s;
            }
        }
        else
        {
            crow::json::wvalue error;
            error["success"] = false;
            error["message"] = "stadiums must be an array of strings";
            return crow::response(400, error.dump());
        }

        // Put path into the cart (converts strings to Stadium)
        backend.addPathToCart(stadiumNames);
        vector<Stadium> updatedPath = backend.getCartPath();

        crow::json::wvalue res;
        res = stadiumListToJson(updatedPath);
        cout << endl << endl << endl << "res" << res.dump() << endl << endl << endl;
        return crow::response(res.dump());
    });
}