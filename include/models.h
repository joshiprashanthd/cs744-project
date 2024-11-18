#pragma once

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace messagebrokerv1
{
    class ServiceRegisterRequestDTO
    {
    private:
        std::string name;
        std::string scope;

    public:
        ServiceRegisterRequestDTO() = default;
        ServiceRegisterRequestDTO(const std::string name, const std::string scope) : name(name), scope(scope)
        {
        }
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(ServiceRegisterRequestDTO, name, scope)
        std::string getName()
        {
            return name;
        }
        std::string getScope()
        {
            return scope;
        }
    };

    class ServiceRegisterResponseDTO
    {
    private:
        std::string id;
        std::string name;
        std::string scope;

    public:
        ServiceRegisterResponseDTO() = default;
        ServiceRegisterResponseDTO(const std::string id, const std::string name, const std::string scope) : id(id), name(name), scope(scope) {}
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(ServiceRegisterResponseDTO, id, name, scope)
        std::string getId()
        {
            return id;
        }
        std::string getName()
        {
            return name;
        }
        std::string getScope()
        {
            return scope;
        }
    };
}