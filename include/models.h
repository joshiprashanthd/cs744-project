#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <nlohmann/json.hpp>
#include "utils.h"
using json = nlohmann::json;

namespace messagebrokerv1
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
        Service(const std::string id, const std::string name, const std::string scope) : id(id), name(name), scope(scope) {}
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Service, id, name, scope, topics, subscriptions)
        void add_topic(std::string topic)
        {
            topics.push_back(topic);
        }
        void add_subscription(std::string subscriptionId)
        {
            subscriptions.push_back(subscriptionId);
        }
    };

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
        TopicPublishRequestDTO(const std::string name) : name(name) {}
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(TopicPublishRequestDTO, name)
        std::string getName()
        {
            return name;
        }
    };

    class TopicSubscriptionRequestDTO
    {
    private:
        std::string name;

    public:
        TopicSubscriptionRequestDTO() = default;
        TopicSubscriptionRequestDTO(const std::string name) : name(name) {}
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(TopicSubscriptionRequestDTO, name)
        std::string getName()
        {
            return name;
        }
    };

    class GenericResponseDTO
    {
    private:
        bool success;

    public:
        GenericResponseDTO() = default;
        GenericResponseDTO(bool success) : success(success) {}
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(GenericResponseDTO, success)
    };

    class Message
    {
    private:
        std::string messageId;
        std::string payload;
        std::string createdTimestamp;

    public:
        Message() : createdTimestamp(getCurrentDateTime()) {}
        Message(std::string messageId, std::string payload) : messageId(messageId), payload(payload), createdTimestamp(getCurrentDateTime()) {}
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Message, messageId, payload, createdTimestamp)
        std::string getMessageId()
        {
            return messageId;
        }
        std::string getPayload()
        {
            return payload;
        }
        std::string getCreatedTimestamp()
        {
            return createdTimestamp;
        }
    };

    class TopicSubscription
    {
    private:
        std::string id;
        std::string name;
        std::string topicName;

    public:
        TopicSubscription() = default;
        TopicSubscription(std::string id, std::string name, std::string topicName) : id(id), name(name), topicName(topicName) {}
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(TopicSubscription, id, name, topicName)
        std::string getId()
        {
            return id;
        }
        std::string getName()
        {
            return name;
        }
        std::string getTopicName()
        {
            return topicName;
        }
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
}