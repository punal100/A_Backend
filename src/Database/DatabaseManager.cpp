#include "DatabaseManager.hpp"
#include <sstream>

DatabaseManager &DatabaseManager::GetInstance()
{
    static DatabaseManager instance;
    return instance;
}

void DatabaseManager::Connect(const std::string &connectionString)
{
    try
    {
        m_Connection = std::make_unique<pqxx::connection>(connectionString);
        if (m_Connection->is_open())
        {
            std::cout << "Connected to database successfully: " << m_Connection->dbname() << std::endl;

            // Initialize Tables if they don't exist
            pqxx::work W(*m_Connection);
            W.exec0("CREATE TABLE IF NOT EXISTS users (id UUID DEFAULT gen_random_uuid() PRIMARY KEY, username TEXT UNIQUE NOT NULL, password_hash TEXT NOT NULL, roles TEXT[] NOT NULL)");
            W.exec0("CREATE TABLE IF NOT EXISTS player_data (user_id UUID REFERENCES users(id), level INT DEFAULT 1, xp INT DEFAULT 0, PRIMARY KEY (user_id))");
            W.exec0("CREATE TABLE IF NOT EXISTS inventory (id SERIAL PRIMARY KEY, user_id UUID REFERENCES users(id), item_id TEXT NOT NULL, count INT DEFAULT 1)");
            W.commit();
        }
        else
        {
            std::cerr << "Failed to open database" << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Database connection error: " << e.what() << std::endl;
    }
}

pqxx::connection *DatabaseManager::GetConnection()
{
    return m_Connection.get();
}

bool DatabaseManager::CreateUser(const std::string &username, const std::string &password, const std::vector<std::string> &roles)
{
    try
    {
        if (!m_Connection || !m_Connection->is_open())
            return false;

        pqxx::work W(*m_Connection);

        // Convert vector to PostgreSQL array string format: {role1,role2}
        std::string rolesStr = "{";
        for (size_t i = 0; i < roles.size(); ++i)
        {
            rolesStr += roles[i];
            if (i < roles.size() - 1)
                rolesStr += ",";
        }
        rolesStr += "}";

        // In a real app, hash the password here!
        std::string passwordHash = password;

        W.exec_params("INSERT INTO users (username, password_hash, roles) VALUES ($1, $2, $3)", username, passwordHash, rolesStr);
        W.commit();
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "CreateUser error: " << e.what() << std::endl;
        return false;
    }
}

std::optional<User> DatabaseManager::GetUserByUsername(const std::string &username)
{
    try
    {
        if (!m_Connection || !m_Connection->is_open())
            return std::nullopt;

        pqxx::work W(*m_Connection);
        pqxx::result R = W.exec_params("SELECT id, username, password_hash, roles FROM users WHERE username = $1", username);

        if (R.empty())
            return std::nullopt;

        User user;
        user.id = R[0][0].as<std::string>();
        user.username = R[0][1].as<std::string>();
        user.password_hash = R[0][2].as<std::string>();

        ParseRoles(R[0][3].as<std::string>(), user.roles);

        return user;
    }
    catch (const std::exception &e)
    {
        std::cerr << "GetUserByUsername error: " << e.what() << std::endl;
        return std::nullopt;
    }
}

std::optional<User> DatabaseManager::GetUserById(const std::string &id)
{
    try
    {
        if (!m_Connection || !m_Connection->is_open())
            return std::nullopt;

        pqxx::work W(*m_Connection);
        pqxx::result R = W.exec_params("SELECT id, username, password_hash, roles FROM users WHERE id = $1", id);

        if (R.empty())
            return std::nullopt;

        User user;
        user.id = R[0][0].as<std::string>();
        user.username = R[0][1].as<std::string>();
        user.password_hash = R[0][2].as<std::string>();

        ParseRoles(R[0][3].as<std::string>(), user.roles);

        return user;
    }
    catch (const std::exception &e)
    {
        std::cerr << "GetUserById error: " << e.what() << std::endl;
        return std::nullopt;
    }
}

std::optional<PlayerData> DatabaseManager::GetPlayerData(const std::string &userId)
{
    try
    {
        if (!m_Connection || !m_Connection->is_open())
            return std::nullopt;

        pqxx::work W(*m_Connection);
        pqxx::result R = W.exec_params("SELECT user_id, level, xp FROM player_data WHERE user_id = $1", userId);

        if (R.empty())
        {
            // Create default data if not exists
            W.exec_params("INSERT INTO player_data (user_id) VALUES ($1)", userId);
            W.commit();
            return PlayerData{userId, 1, 0};
        }

        PlayerData data;
        data.userId = R[0][0].as<std::string>();
        data.level = R[0][1].as<int>();
        data.xp = R[0][2].as<int>();
        return data;
    }
    catch (const std::exception &e)
    {
        std::cerr << "GetPlayerData error: " << e.what() << std::endl;
        return std::nullopt;
    }
}

bool DatabaseManager::UpdatePlayerData(const std::string &userId, int level, int xp)
{
    try
    {
        if (!m_Connection || !m_Connection->is_open())
            return false;
        pqxx::work W(*m_Connection);
        W.exec_params("UPDATE player_data SET level = $2, xp = $3 WHERE user_id = $1", userId, level, xp);
        W.commit();
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "UpdatePlayerData error: " << e.what() << std::endl;
        return false;
    }
}

std::vector<std::string> DatabaseManager::GetPlayerInventory(const std::string &userId)
{
    std::vector<std::string> items;
    try
    {
        if (!m_Connection || !m_Connection->is_open())
            return items;
        pqxx::work W(*m_Connection);
        pqxx::result R = W.exec_params("SELECT item_id, count FROM inventory WHERE user_id = $1", userId);
        for (const auto &row : R)
        {
            std::string item = row[0].as<std::string>();
            int count = row[1].as<int>();
            items.push_back(item + " (x" + std::to_string(count) + ")");
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "GetPlayerInventory error: " << e.what() << std::endl;
    }
    return items;
}

bool DatabaseManager::AddItemToInventory(const std::string &userId, const std::string &itemId, int count)
{
    try
    {
        if (!m_Connection || !m_Connection->is_open())
            return false;
        pqxx::work W(*m_Connection);
        pqxx::result R = W.exec_params("SELECT id, count FROM inventory WHERE user_id = $1 AND item_id = $2", userId, itemId);
        if (R.empty())
        {
            W.exec_params("INSERT INTO inventory (user_id, item_id, count) VALUES ($1, $2, $3)", userId, itemId, count);
        }
        else
        {
            int currentCount = R[0][1].as<int>();
            W.exec_params("UPDATE inventory SET count = $2 WHERE id = $1", R[0][0].as<int>(), currentCount + count);
        }
        W.commit();
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "AddItemToInventory error: " << e.what() << std::endl;
        return false;
    }
}

std::vector<User> DatabaseManager::GetAllUsers()
{
    std::vector<User> users;
    try
    {
        if (!m_Connection || !m_Connection->is_open())
            return users;

        pqxx::work W(*m_Connection);
        pqxx::result R = W.exec("SELECT id, username, password_hash, roles FROM users");

        for (const auto &row : R)
        {
            User user;
            user.id = row[0].as<std::string>();
            user.username = row[1].as<std::string>();
            user.password_hash = row[2].as<std::string>();

            ParseRoles(row[3].as<std::string>(), user.roles);
            users.push_back(user);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "GetAllUsers error: " << e.what() << std::endl;
    }
    return users;
}

void DatabaseManager::ParseRoles(std::string rolesStr, std::vector<std::string> &roles)
{
    // The roles are stored in postgres as an array, which pqxx reads as a string like "{role1,role2}"
    // This function parses that string into a vector of role strings.
    if (rolesStr.size() > 2) // Check if it's not just "{}"
    {
        // Remove the curly braces
        rolesStr = rolesStr.substr(1, rolesStr.size() - 2);
        std::string delimiter = ",";
        size_t pos = 0;
        std::string token;
        // Split the string by the delimiter
        while ((pos = rolesStr.find(delimiter)) != std::string::npos)
        {
            token = rolesStr.substr(0, pos);
            roles.push_back(token);
            rolesStr.erase(0, pos + delimiter.length());
        }
        roles.push_back(rolesStr); // Add the last role
    }
}