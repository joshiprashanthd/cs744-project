#pragma once

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace publisherv1
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

    class TopicPublishRequestDTO
    {
    private:
        std::string name;

    public:
        TopicPublishRequestDTO() = default;
        TopicPublishRequestDTO(const std::string topicName) : name(topicName) {}
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(TopicPublishRequestDTO, name)
        std::string getName()
        {
            return name;
        }
    };

    class SendMessageDTO
    {
    private:
        std::string messageId;
        std::string payload;

    public:
        SendMessageDTO() = default;
        SendMessageDTO(const std::string messageId, const std::string payload) : messageId(messageId), payload(payload) {}
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(SendMessageDTO, messageId, payload);
    };

    class GenericResponseDTO
    {
    private:
        std::string message;
        bool success;

    public:
        GenericResponseDTO() = default;
        GenericResponseDTO(std::string message, bool success) : message(message), success(success) {}
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(GenericResponseDTO, success, message)
    };
}