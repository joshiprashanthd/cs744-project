#pragma once

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace publisherv1
{
    class Service
    {
    private:
        std::string id;
        std::string name;
        std::string scope;
        std::vector<std::string> topics;
        std::vector<std::string> subscriptions;

    public:
        Service() = default;
        Service(const std::string id, const std::string name, const std::string scope) : id(id), name(name), scope(scope) {}
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Service, id, name, scope, topics, subscriptions)
        std::string getId()
        {
            return id;
        }
    };
}