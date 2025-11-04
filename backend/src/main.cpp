#include "models/backendManager.h"

// Windows-specific define needed by Asio
#define _WIN32_WINNT 0x0601
#include <asio.hpp>
#include <crow.h>

int main()
{
    BackendManager backend;
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        return "Hello world";
    });

    app.port(18080).multithreaded().run();
    return 0;
}