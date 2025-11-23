#pragma once

#include <string>
#include <vector>
#include <crow/json.h>

struct User {
    std::string id;
    std::string username;
    std::string password_hash;
    std::vector<std::string> roles;

    crow::json::wvalue ToJson() const {
        crow::json::wvalue json;
        json["id"] = id;
        json["username"] = username;
        for (size_t i = 0; i < roles.size(); ++i) {
            json["roles"][i] = roles[i];
        }
        return json;
    }
};