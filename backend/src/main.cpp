#include "../include/models/backendManager.h"

// Windows-specific define needed by Asio
// #define _WIN32_WINNT 0x0601
// #include <asio.hpp>
// #include <crow.h>
// #include <nlohmann/json.hpp>

#include "crow.h"

int main()
{
    BackendManager backend;

    crow::SimpleApp app;

    // Test
    CROW_ROUTE(app, "/")([]() {
        return "Hello World";
    });

    /***********************************************************************
     * ADMIN LOGIN - backendManager.cpp
     **********************************************************************/
    // CROW_ROUTE(app, "/api/login").methods("POST"_method)
    // ([&backend](const crow::request& req){
    //     try {
    //         auto body = nlohmann::json::parse(req.body);
    //         std::string user = body.value("username", "");
    //         std::string pass = body.value("password", "");
    //         bool login_success = backend.login(user, pass);

    //         if (login_success) {
    //             crow::response res(R"({ "success": true })");
    //             res.set_header("Content-Type","application/json");
    //             return res;
    //         } else {
    //             return crow::response(401);
    //         }
    //     } catch (std::exception& ex) {
    //         std::cerr << "CAUGHT: " << ex.what();
    //         return crow::response(400);
    //     }
    // });

    // CROW_ROUTE(app, "/api/login")
    // ([&backend](const crow::request& req){
    //     try {
    //         auto params = req.url_params;

    //         std::string user;
    //         std::string pass;

    //         auto keys = params.keys();
    //         if (std::find(keys.begin(), keys.end(), "username") != keys.end()) {
    //             user = params.get("username");
    //             std::cerr << "Got username: " << user << '\n';
    //         }
    //         if (std::find(keys.begin(), keys.end(), "password") != keys.end()) {
    //             pass = params.get("password");
    //             std::cerr << "Got password: " << pass << '\n';

    //         }
    //         bool login_success = backend.login(user, pass);
            
    //         if (login_success) {
    //             crow::response res(R"(<h1>LOGGED ON!!!!</h1>)");
    //             res.set_header("Content-Type","text/html");
    //             return res;
    //         } else {
    //             return crow::response(401);
    //         }
    //     } catch (std::exception& ex) {
    //         std::cerr << "CAUGHT: " << ex.what() << '\n';
    //         return crow::response(400);
    //     }
    // });
    // End ADMIN LOGIN

    // Port and run
    app.port(18080).multithreaded().run();

    return 0;
}