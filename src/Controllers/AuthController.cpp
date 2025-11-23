#include "AuthController.hpp"
#include "../Models/User.hpp"
#include <ctime>

void AuthController::RegisterRoutes(crow::Blueprint &bp)
{
    // Note: We use CROW_BP_ROUTE instead of CROW_ROUTE
    // The "/api" prefix is handled in main.cpp, so we just use "/login" here.

    CROW_BP_ROUTE(bp, "/login").methods(crow::HTTPMethod::POST)([](const crow::request &req)
                                                                {
        auto x = crow::json::load(req.body);
        if (!x) return crow::response(400, "Invalid JSON");
        
        std::string username = x["username"].s();
        std::string password = x["password"].s();

        auto userOpt = DatabaseManager::GetInstance().GetUserByUsername(username);
        if (!userOpt) return crow::response(401, "User not found");
        
        // In production, compare Hashes!
        if (userOpt->password_hash != password) return crow::response(401, "Invalid password");

        crow::json::wvalue result;
        result["token"] = "session_" + userOpt->id; 
        result["userId"] = userOpt->id;
        result["message"] = "Login Successful";

        crow::json::wvalue rolesJson;
        for(size_t i=0; i<userOpt->roles.size(); ++i) rolesJson[i] = userOpt->roles[i];
        result["roles"] = std::move(rolesJson);

        return crow::response(result); });

    CROW_BP_ROUTE(bp, "/register").methods(crow::HTTPMethod::POST)([](const crow::request &req)
                                                                   {
        auto x = crow::json::load(req.body);
        if (!x) return crow::response(400, "Invalid JSON");

        std::string username = x["username"].s();
        std::string password = x["password"].s();
        
        std::vector<std::string> roles = {"Regular"};
        if (x.has("roles")) {
            // Optional: Parse roles from request if admin
        }

        if (DatabaseManager::GetInstance().CreateUser(username, password, roles)) {
            return crow::response(200, "Registered Successfully");
        } else {
            return crow::response(500, "Registration Failed");
        } });

    // --- CRITICAL UNREAL VALIDATION LOGIC ---
    CROW_BP_ROUTE(bp, "/validate").methods(crow::HTTPMethod::POST)([](const crow::request &req)
                                                                   {
        auto x = crow::json::load(req.body);
        if (!x) return crow::response(400, "Invalid JSON");

        std::string token = x["token"].s();
        std::string userId = x["userId"].s();

        // 1. Validate Token
        if (token != "session_" + userId) {
             return crow::response(401, "Invalid Token");
        }

        // 2. Fetch User & Roles
        auto userOpt = DatabaseManager::GetInstance().GetUserById(userId);
        if (!userOpt) {
            return crow::response(401, "User ID not found");
        }

        crow::json::wvalue result;
        result["valid"] = true;
        result["userId"] = userOpt->id;

        // 3. Return Roles for Unreal Subsystem
        crow::json::wvalue rolesJson;
        for(size_t i=0; i<userOpt->roles.size(); ++i) {
            rolesJson[i] = userOpt->roles[i];
        }
        result["roles"] = std::move(rolesJson);

        return crow::response(result); });

    CROW_BP_ROUTE(bp, "/users").methods(crow::HTTPMethod::GET)([]()
                                                               {
        auto users = DatabaseManager::GetInstance().GetAllUsers();
        crow::json::wvalue json_users;
        for (size_t i = 0; i < users.size(); ++i)
        {
            json_users[i]["id"] = users[i].id;
            json_users[i]["username"] = users[i].username;
            crow::json::wvalue rolesJson;
            for(size_t j=0; j<users[i].roles.size(); ++j) rolesJson[j] = users[i].roles[j];
            json_users[i]["roles"] = std::move(rolesJson);
        }
        return crow::response(json_users); });
}