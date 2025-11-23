#pragma once

#include <crow.h>
#include "../Database/DatabaseManager.hpp"

class AuthController
{
public:
    // CHANGED: Now accepts a Blueprint reference
    static void RegisterRoutes(crow::Blueprint &bp);
};