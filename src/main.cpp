#include <crow.h>
// 1. You MUST include the CORS middleware header
#include <crow/middlewares/cors.h>

#include "Database/DatabaseManager.hpp"
#include "Controllers/AuthController.hpp"
#include "Controllers/PlayerController.hpp"

int main()
{
    // Connect to Database
    const char *dbConnEnv = std::getenv("DB_CONNECTION_STRING");
    std::string dbConnString = dbConnEnv ? dbConnEnv : "postgresql://postgres:password@localhost:5432/a_server_db";

    DatabaseManager::GetInstance().Connect(dbConnString);

    // 2. CHANGE: Use crow::App<crow::CORSHandler> instead of SimpleApp
    // This allows us to inject the CORS middleware.
    crow::App<crow::CORSHandler> app;

    // 3. CONFIGURE CORS
    // This allows your React Frontend (on any port) to talk to this Backend
    auto &cors = app.get_middleware<crow::CORSHandler>();
    cors.global()
        .headers("Content-Type", "Authorization", "X-Custom-Header")
        .methods("POST"_method, "GET"_method, "OPTIONS"_method)
        .origin("*"); // WARNING: In production, replace "*" with your specific frontend domain

    // Create a Blueprint to group routes under the "/api" prefix
    crow::Blueprint api_blueprint("api");

    // Register Routes
    AuthController::RegisterRoutes(api_blueprint);
    PlayerController::RegisterRoutes(api_blueprint);
    app.register_blueprint(api_blueprint);

    // 4. CHANGE: Port set to 18080 to match your axios.js
    std::cout << "Starting A_Backend Server on port 18080..." << std::endl;
    app.port(18080).multithreaded().run();

    return 0;
}