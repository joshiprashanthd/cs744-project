#pragma once

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace subscriberv1
{
    class GenericResponseDTO
    {
    private:
        std::string message;
        bool success;

    public:
        GenericResponseDTO() = default;
        GenericResponseDTO(bool success, std::string message) : success(success), message(message) {}
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(GenericResponseDTO, success, message)
    };
}