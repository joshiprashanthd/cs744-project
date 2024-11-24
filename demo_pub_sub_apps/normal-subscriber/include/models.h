#pragma once

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace subscriberv1
{
    class Message
    {
    private:
        std::string messageId;
        std::string payload;
        std::string createdTimestamp;

    public:
        Message() = default;
        Message(std::string messageId, std::string payload, std::string createdTimestamp) : messageId(messageId), payload(payload), createdTimestamp(createdTimestamp) {}
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
}