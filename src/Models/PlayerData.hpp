#pragma once

#include <string>
#include <crow/json.h>

struct PlayerData {
    std::string userId;
    int level;
    int xp;

    crow::json::wvalue ToJson() const {
        crow::json::wvalue json;
        json["userId"] = userId;
        json["level"] = level;
        json["xp"] = xp;
        return json;
    }
};