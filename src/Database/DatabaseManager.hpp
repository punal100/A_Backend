#pragma once

#include <pqxx/pqxx>
#include <string>
#include <memory>
#include <iostream>
#include <optional>
#include "../Models/User.hpp"
#include "../Models/PlayerData.hpp"

class DatabaseManager
{
public:
    static DatabaseManager &GetInstance();
    void Connect(const std::string &connectionString);
    pqxx::connection *GetConnection();

    // User Methods
    bool CreateUser(const std::string &username, const std::string &password, const std::vector<std::string> &roles);
    std::optional<User> GetUserByUsername(const std::string &username);

    // [NEW] Required for Token Validation
    std::optional<User> GetUserById(const std::string &id);

    std::vector<User> GetAllUsers();

    // Player Data Methods
    std::optional<PlayerData> GetPlayerData(const std::string &userId);
    bool UpdatePlayerData(const std::string &userId, int level, int xp);

    // Inventory Methods
    std::vector<std::string> GetPlayerInventory(const std::string &userId);
    bool AddItemToInventory(const std::string &userId, const std::string &itemId, int count);

private:
    DatabaseManager() = default;
    ~DatabaseManager() = default;
    DatabaseManager(const DatabaseManager &) = delete;
    DatabaseManager &operator=(const DatabaseManager &) = delete;

    void ParseRoles(std::string rolesStr, std::vector<std::string> &roles);
    std::unique_ptr<pqxx::connection> m_Connection;
};