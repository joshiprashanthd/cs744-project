#include <iostream>
#include "models.h"
#include "HTTPRequest.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>

const std::string PLATFORM_BASE_URL = "http://localhost:3000/api/v1";

std::vector<subscriberv1::Message> pollMessages(std::string serviceId, std::string topicName, std::string subscriptionName);

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <topic_name> <subscription_name>\n", argv[0]);
        exit(1);
    }

    std::cout << "Subscriber started!" << std::endl;
    std::string serviceId = "6742bb3a6d07c062720981d2";
    std::string topicName = argv[1];
    std::string subscriptionName = argv[2];

    while (true)
    {
        sleep(5);
        auto messsages = pollMessages(serviceId, topicName, subscriptionName);
        if (messsages.size() > 0)
        {
            std::cout << messsages.size() << " new messages received!" << std::endl;
            for (auto message : messsages)
            {
                auto now = std::chrono::system_clock::now();
                std::time_t current_time = std::chrono::system_clock::to_time_t(now);
                json message_json = message;
                std::cout << "MessageId: " << message.getMessageId() << std::endl;
                std::cout << "Message Received at time: " << std::put_time(std::localtime(&current_time), "%Y-%m-%d %H:%M:%S");
                std::cout << "Full Message: " << message_json.dump() << std::endl;
            }
        }
        else
        {
            std::cout << "No messages to poll!" << std::endl;
        }
    }
}

std::vector<subscriberv1::Message> pollMessages(std::string serviceId, std::string topicName, std::string subscriptionName)
{
    http::Request request{PLATFORM_BASE_URL + "/services/" + serviceId + "/topics/" + topicName + "/subscriptions/" + subscriptionName + "/messages"};
    const auto response = request.send("GET");
    const auto content_str = std::string{response.body.begin(), response.body.end()};
    std::cout << content_str << std::endl;
    if (content_str.length() > 0)
    {
        auto messages_json = json::parse(content_str);
        auto messages = messages_json.template get<std::vector<subscriberv1::Message>>();
        return messages;
    }

    return {};
}