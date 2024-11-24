#pragma once

#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include "civetweb.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace messagebrokerv1
{
    std::string getCurrentDateTime();
    std::string formatLogMessage(std::string message);
    bool sendJsonResponse(struct mg_connection *conn, json response);
    int logMessage(const struct mg_connection *conn, const char *message);
}