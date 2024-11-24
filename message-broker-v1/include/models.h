#pragma once

#include <iostream>
#include "utils.h"
#include <nlohmann/json.hpp>
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

    class Subscription
    {
    private:
        std::string name;
        std::string topicName;
        std::string type;
        std::string webhookUrl;

    public:
        Subscription() = default;
        Subscription(const std::string name, const std::string topicName, const std::string type, std::string webhoolUrl = "") : name(name), topicName(topicName), type(type), webhookUrl(webhookUrl) {}
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Subscription, name, topicName, type, webhookUrl);
        std::string getName() { return name; }
        std::string getTopicName() { return topicName; }
        std::string getSubscriptionType() { return type; }
        std::string getWebhookUrl() { return webhookUrl; }
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

} // namespace messagebrokerv1
