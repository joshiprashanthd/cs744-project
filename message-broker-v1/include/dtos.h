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

    class SubscriptionRequestDTO
    {
    private:
        std::string name;
        std::string topicName;
        std::string subscriptionType;

    public:
        SubscriptionRequestDTO() = default;
        SubscriptionRequestDTO(const std::string name, std::string topicName, std::string subscriptionType) : name(name), topicName(topicName), subscriptionType(subscriptionType) {}
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(SubscriptionRequestDTO, name, topicName, subscriptionType);
        std::string getName() { return name; }
        std::string getTopicName() { return topicName; }
        std::string getSubscriptionType() { return subscriptionType; }
    };

    class TopicPublishRequestDTO
    {
    private:
        std::string name;

    public:
        TopicPublishRequestDTO() = default;
        TopicPublishRequestDTO(const std::string name) : name(name) {}
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(TopicPublishRequestDTO, name)
        std::string getName()
        {
            return name;
        }
    };

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

    class MessageSendDTO
    {
    private:
        std::string messageId;
        std::string payload;

    public:
        MessageSendDTO() = default;
        MessageSendDTO(std::string messageId, std::string payload) {}
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(MessageSendDTO, messageId, payload)
        std::string getMessageId()
        {
            return messageId;
        }
        std::string getPayload()
        {
            return payload;
        }
    };

    class TopicSubscriptionRequestDTO
    {
    private:
        std::string subscriptionName;
        std::string topicName;
        std::string subscriptionType;
        std::string webhookUrl;

    public:
        TopicSubscriptionRequestDTO() = default;
        TopicSubscriptionRequestDTO(const std::string subscriptionName, std::string topicName, std::string subscriptionType, std::string webhookUrl = "") : subscriptionName(subscriptionName), topicName(topicName), subscriptionType(subscriptionType), webhookUrl(webhookUrl) {}
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(TopicSubscriptionRequestDTO, subscriptionName, topicName, subscriptionType, webhookUrl);
        std::string getSubscriptionName()
        {
            return subscriptionName;
        }
        std::string getTopicName()
        {
            return topicName;
        }
        std::string getSubscriptionType()
        {
            return subscriptionType;
        }
        std::string getWebhookUrl()
        {
            return webhookUrl;
        }
    };
}