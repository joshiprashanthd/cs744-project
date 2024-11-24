#include <iostream>
#include <random>
#include <sstream>
#include <iomanip>
#include "HTTPRequest.hpp"
#include "dtos.h"
#include "models.h"
#include <chrono>
#include <ctime>
#include <iomanip>

const std::string PLATFORM_BASE_URL = "http://localhost:3000/api/v1";

std::string registerServiceInPlatform();
bool publishTopic(std::string serviceId, std::string topicName);
int generateRandomNumber(int n);
std::string generateUUID();
bool sendMessage(std::string serviceId, std::string topicName, std::string messageId, std::string message);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <messages_cnt>\n", argv[0]);
        exit(1);
    }

    std::cout << "Publisher started!" << std::endl;

    std::string serviceId = "67409702e00595d0500e7a72";
    std::vector<std::string> topics{"test_topic_1", "test_topic_2"};

    int messages_cnt = atoi(argv[1]);
    std::cout << "Total Messages to Send: " << messages_cnt << std::endl;
    int i = 0;
    int topicOneMessagesCnt = 0, topicTwoMessagesCnt = 0;
    while (i <= messages_cnt)
    {
        int idx = generateRandomNumber(topics.size() - 1);
        std::cout << "Choosing topic: " << topics[idx] << std::endl;
        auto message_id = generateUUID();
        std::cout << "[MessageId: " << message_id << "] + Sending message: test message " << i << std::endl;
        auto strMsgId = std::to_string(i);
        sendMessage(serviceId, topics[idx], message_id, "test message " + strMsgId);
        auto now = std::chrono::system_clock::now();
        std::time_t current_time = std::chrono::system_clock::to_time_t(now);

        std::cout << std::put_time(std::localtime(&current_time), "%Y-%m-%d %H:%M:%S") << "[MessageId: " << message_id << "]" << " Message sent!" << std::endl;
        if (idx == 0)
        {
            topicOneMessagesCnt++;
        }
        else
        {
            topicTwoMessagesCnt++;
        }
        i++;
        sleep(2);
    }

    std::cout << "Publisher stopped!" << std::endl;
    std::cout << "Total Messages sent to " << topics[0] << " are " << topicOneMessagesCnt << std::endl;
    std::cout << "Total Messages sent to " << topics[1] << " are " << topicTwoMessagesCnt << std::endl;

    return EXIT_SUCCESS;
}

std::string registerServiceInPlatform()
{
    http::Request request{PLATFORM_BASE_URL + "/services/register"};
    publisherv1::ServiceRegisterRequestDTO service_request("test_pub_1", "test");
    json json_body = service_request;
    const std::string body = json_body.dump();
    const auto response = request.send("POST", body, {{"Content-Type", "application/json"}});
    const auto content_str = std::string{response.body.begin(), response.body.end()};
    auto service_json = json::parse(content_str);
    auto service = service_json.template get<publisherv1::Service>();

    return service.getId();
}

bool publishTopic(std::string serviceId, std::string topicName)
{
    http::Request request{PLATFORM_BASE_URL + "/services/" + serviceId + "/topics/publish"};
    publisherv1::TopicPublishRequestDTO service_request(topicName);
    json json_body = service_request;
    const std::string body = json_body.dump();
    const auto response = request.send("POST", body, {{"Content-Type", "application/json"}});

    return response.status.code == http::Status::Ok;
}

bool sendMessage(std::string serviceId, std::string topicName, std::string messageId, std::string message)
{
    http::Request request{PLATFORM_BASE_URL + "/services/" + serviceId + "/topics/" + topicName + "/send"};
    publisherv1::SendMessageDTO send_message_request(messageId, message);
    json json_body = send_message_request;
    const std::string body = json_body.dump();
    const auto response = request.send("POST", body, {{"Content-Type", "application/json"}});

    return response.status.code == http::Status::Ok;
}

int generateRandomNumber(int n)
{
    // Create a random number generator
    std::random_device rd;                         // Seed for random number engine
    std::mt19937 gen(rd());                        // Mersenne Twister RNG
    std::uniform_int_distribution<> distrib(0, n); // Uniform distribution [0, n]

    // Generate random number
    int random_number = distrib(gen);

    return random_number;
}

std::string generateUUID()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 15);        // For hexadecimal values (0-F)
    std::uniform_int_distribution<> distribVariant(8, 11); // For UUID variant

    std::ostringstream uuid;

    for (int i = 0; i < 8; ++i)
        uuid << std::hex << distrib(gen);

    uuid << "-";

    for (int i = 0; i < 4; ++i)
        uuid << std::hex << distrib(gen);

    uuid << "-4"; // UUID version 4 (randomly generated)
    for (int i = 0; i < 3; ++i)
        uuid << std::hex << distrib(gen);

    uuid << "-";
    uuid << std::hex << distribVariant(gen); // UUID variant
    for (int i = 0; i < 3; ++i)
        uuid << std::hex << distrib(gen);

    uuid << "-";

    for (int i = 0; i < 12; ++i)
        uuid << std::hex << distrib(gen);

    return uuid.str();
}