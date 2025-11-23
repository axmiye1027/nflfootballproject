// routes.cpp

#include "routes/routes.h"

void registerRoutes(crow::SimpleApp& app)
{
    // respond to OPTIONS preflight
    CROW_ROUTE(app, "/hello").methods("OPTIONS"_method)([]() {
        crow::response r;
        r.add_header("Access-Control-Allow-Origin", "*");
        r.add_header("Access-Control-Allow-Methods", "GET, OPTIONS");
        r.add_header("Access-Control-Allow-Headers", "*");
        r.code = 200;
        return r;
    });

    CROW_ROUTE(app, "/hello")([]() {
        crow::response r("Hello World");
        r.add_header("Access-Control-Allow-Origin", "*");
        return r;
    });
}