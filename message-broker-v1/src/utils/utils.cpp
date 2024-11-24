#include "utils.h"

namespace messagebrokerv1
{
    std::string getCurrentDateTime()
    {
        // Get the current time
        auto now = std::chrono::system_clock::now();
        // Convert to time_t
        std::time_t timeNow = std::chrono::system_clock::to_time_t(now);

        // Convert to local time
        std::tm localTime;
#ifdef _WIN32
        localtime_s(&localTime, &timeNow); // Windows
#else
        localtime_r(&timeNow, &localTime); // Linux/Unix
#endif

        // Create a stringstream to format the datetime
        std::ostringstream oss;
        oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");

        return oss.str();
    }

    std::string formatLogMessage(std::string message)
    {
        return getCurrentDateTime() + ": " + message;
    }

    bool sendJsonResponse(struct mg_connection *conn, json response)
    {
        std::string response_str = response.dump();
        size_t content_length = response_str.size();

        mg_send_http_ok(conn, "application/json; charset=utf-8", content_length + 1);

        return mg_write(conn, response_str.c_str(), content_length) > 0;
    }

    int logMessage(const struct mg_connection *conn, const char *message)
    {
        puts(message);
        return 1;
    }
}