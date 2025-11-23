#include "PlayerController.hpp"
#include "../Database/DatabaseManager.hpp"
#include "../Models/PlayerData.hpp"

void PlayerController::RegisterRoutes(crow::Blueprint &bp)
{
    // The "api" prefix is added in main, so we start with "/player..."

    CROW_BP_ROUTE(bp, "/player/<string>/inventory").methods(crow::HTTPMethod::GET)([](const crow::request &req, std::string playerId)
                                                                                   {
        auto items = DatabaseManager::GetInstance().GetPlayerInventory(playerId);
        crow::json::wvalue result;
        result["playerId"] = playerId;
        for (size_t i = 0; i < items.size(); ++i) {
            result["items"][i] = items[i];
        }
        return crow::response(result); });

    CROW_BP_ROUTE(bp, "/player/<string>/inventory/add").methods(crow::HTTPMethod::POST)([](const crow::request &req, std::string playerId)
                                                                                        {
        auto x = crow::json::load(req.body);
        if (!x) return crow::response(400, "Invalid JSON");
        
        std::string itemId = x["itemId"].s();
        int count = x.has("count") ? x["count"].i() : 1;

        if (DatabaseManager::GetInstance().AddItemToInventory(playerId, itemId, count)) {
            return crow::response(200, "Item Added");
        } else {
            return crow::response(500, "Failed to add item");
        } });

    CROW_BP_ROUTE(bp, "/player/<string>/data").methods(crow::HTTPMethod::GET, crow::HTTPMethod::POST)([](const crow::request &req, std::string playerId)
                                                                                                      {
        if (req.method == crow::HTTPMethod::GET) {
            auto dataOpt = DatabaseManager::GetInstance().GetPlayerData(playerId);
            if (dataOpt) {
                return crow::response(dataOpt->ToJson());
            } else {
                return crow::response(404, "Player Data Not Found");
            }
        } else if (req.method == crow::HTTPMethod::POST) {
            auto x = crow::json::load(req.body);
            if (!x) return crow::response(400, "Invalid JSON");

            int level = x["level"].i();
            int xp = x["xp"].i();

            if (DatabaseManager::GetInstance().UpdatePlayerData(playerId, level, xp)) {
                return crow::response(200, "Player Data Updated");
            } else {
                return crow::response(500, "Failed to update player data");
            }
        }
        return crow::response(405, "Method Not Allowed"); });
}