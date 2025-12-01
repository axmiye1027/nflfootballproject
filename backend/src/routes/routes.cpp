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

    // include souvenir list for this stadium
    try {
        auto sv = stadium.getSouvenirList().getValues();
        auto& sarr = json["souvenirs"];
        sarr = crow::json::wvalue::list(sv.size());
        for (int i = 0; i < sv.size(); ++i) {
            sarr[i]["souvenirId"] = sv[i].souvenirId;
            sarr[i]["souvenirName"] = sv[i].souvenirName;
            sarr[i]["souvenirPrice"] = sv[i].souvenirPrice;
        }
    } catch (const std::exception& ex) {
        std::cerr << "stadiumToJson: failed to include souvenirs: " << ex.what() << '\n';
    }

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
        r.add_header("Access-Control-Allow-Origin", "http://localhost:5173");
        r.add_header("Access-Control-Allow-Methods", "GET, OPTIONS");
        r.add_header("Access-Control-Allow-Headers", "Content-Type");
        r.add_header("Access-Control-Allow-Credentials", "true");
        r.code = 200;
        return r;
    });

    CROW_ROUTE(app, "/stadiums").methods(crow::HTTPMethod::GET)
    ([&backend] 
    {
        auto stadiums = backend.getStadiumsAsVector();
        auto json     = stadiumListToJson(stadiums);

        crow::response r(json.dump());
        r.add_header("Access-Control-Allow-Origin", "http://localhost:5173");
        r.add_header("Access-Control-Allow-Credentials", "true");
        r.add_header("Content-Type", "application/json");
        return r;
    });

    // Get souvenirs for a single stadium
    CROW_ROUTE(app, "/stadiums/<int>/souvenirs").methods(crow::HTTPMethod::OPTIONS)
    ([](int) {
        crow::response r;
        r.add_header("Access-Control-Allow-Origin", "http://localhost:5173");
        r.add_header("Access-Control-Allow-Methods", "GET, OPTIONS");
        r.add_header("Access-Control-Allow-Headers", "Content-Type");
        r.add_header("Access-Control-Allow-Credentials", "true");
        r.code = 200;
        return r;
    });

    CROW_ROUTE(app, "/stadiums/<int>/souvenirs").methods(crow::HTTPMethod::GET)
    ([&backend](int stadiumId)
    {
        try {
            auto stadiums = backend.getStadiumsAsVector();
            // find stadium by id
            vector<Souvenir> list;
            for (auto &s : stadiums) {
                if (s.getStadiumId() == stadiumId) {
                    list = s.getSouvenirList().getValues();
                    break;
                }
            }

            crow::json::wvalue out;
            auto& arr = out["souvenirs"];
            arr = crow::json::wvalue::list(list.size());
            for (int i = 0; i < list.size(); ++i) {
                arr[i]["souvenirId"] = list[i].souvenirId;
                arr[i]["souvenirName"] = list[i].souvenirName;
                arr[i]["souvenirPrice"] = list[i].souvenirPrice;
            }

            crow::response r(out.dump());
            r.add_header("Access-Control-Allow-Origin", "http://localhost:5173");
            r.add_header("Access-Control-Allow-Credentials", "true");
            r.add_header("Content-Type", "application/json");
            return r;
        } catch (const std::exception& ex) {
            std::cerr << "Souvenirs route error: " << ex.what() << '\n';
            return crow::response(500);
        }
    });

    // Compute distances for an ordered list of stadiumIds
    CROW_ROUTE(app, "/trip/distances").methods(crow::HTTPMethod::OPTIONS)
    ([]() {
        crow::response r;
        r.add_header("Access-Control-Allow-Origin", "http://localhost:5173");
        r.add_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        r.add_header("Access-Control-Allow-Headers", "Content-Type");
        r.add_header("Access-Control-Allow-Credentials", "true");
        r.code = 200;
        return r;
    });

    CROW_ROUTE(app, "/trip/distances").methods(crow::HTTPMethod::POST)
    ([&backend](const crow::request& req)
    {
        try {
            auto body = crow::json::load(req.body);
            if (!body || !body.has("stadiumIds")) {
                return crow::response(400);
            }

            auto ids = body["stadiumIds"];
            if (ids.t() != crow::json::type::List) return crow::response(400);

            vector<int> stadiumIds;
            for (auto &v : ids) stadiumIds.push_back((int)v.u());

            crow::json::wvalue out;
            auto& legs = out["legs"];
            legs = crow::json::wvalue::list(max(0, (int)stadiumIds.size() - 1));

            int total = 0;
            for (int i = 0; i + 1 < stadiumIds.size(); ++i) {
                int a = stadiumIds[i];
                int b = stadiumIds[i+1];
                int dist = backend.getDistanceBetween(a, b);
                if (dist < 0) dist = 0;
                legs[i] = dist;
                total += dist;
            }
            out["total"] = total;

            crow::response r(out.dump());
            r.add_header("Access-Control-Allow-Origin", "http://localhost:5173");
            r.add_header("Access-Control-Allow-Credentials", "true");
            r.add_header("Content-Type", "application/json");
            return r;
        } catch (const std::exception& ex) {
            std::cerr << "Trip distances route error: " << ex.what() << '\n';
            return crow::response(500);
        }
    });

    // LOGIN (options + post)
    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::OPTIONS)
    ([]()
    {
        crow::response r;
        r.add_header("Access-Control-Allow-Origin", "http://localhost:5173");
        r.add_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        r.add_header("Access-Control-Allow-Headers", "Content-Type");
        r.add_header("Access-Control-Allow-Credentials", "true");
        r.code = 200;
        return r;
    });

    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::POST)
    ([&backend](const crow::request& req)
    {
        try {
            auto body = crow::json::load(req.body);
            if (!body) {
                return crow::response(400);
            }

            std::string user = body.has("username") ? body["username"].s() : std::string();
            std::string pass = body.has("password") ? body["password"].s() : std::string();

            bool ok = backend.login(user, pass);

            crow::json::wvalue out;
            out["isAdmin"] = ok && backend.isAdminStatus();

            crow::response r(out.dump());
            r.add_header("Access-Control-Allow-Origin", "http://localhost:5173");
            r.add_header("Access-Control-Allow-Credentials", "true");
            r.add_header("Content-Type", "application/json");

            if (ok) {
                return r;
            } else {
                r.code = 401;
                return r;
            }
        } catch (const std::exception& ex) {
            std::cerr << "Login route error: " << ex.what() << '\n';
            return crow::response(400);
        }
    });


}