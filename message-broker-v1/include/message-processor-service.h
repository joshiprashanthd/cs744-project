#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include "models.h"
#include "dependency-injection-container.hpp"
#include "service-repository.h"
#include "subscription-repository.h"
#include "HTTPRequest.hpp"
#include <thread>
#include <mutex>

namespace messagebrokerv1
{
    class MessageProcessorService
    {
        class TopicSubscriptionQueues
        {
            std::unordered_map<std::string, std::queue<Message>> subscriptionQueues;
            std::unordered_map<std::string, std::queue<Message>> eventSubscriptionQueues;
            std::unordered_map<std::string, std::string> eventSubscriptionWebhookMap;
            pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

        public:
            bool addSubscription(std::string subscriptionName, const std::string &type, std::string webhookUrl = "")
            {
                std::queue<Message> subQueue;
                if (type == "normal")
                    subscriptionQueues[subscriptionName] = subQueue;
                else
                {
                    eventSubscriptionQueues[subscriptionName] = subQueue;
                    eventSubscriptionWebhookMap[subscriptionName] = webhookUrl;
                }

                return true;
            }
            void addMessage(Message message)
            {
                pthread_rwlock_rdlock(&rwlock);
                // Add message to normal subscription queues
                for (auto subscriptionPair : subscriptionQueues)
                {
                    subscriptionQueues[subscriptionPair.first].push(message);
                }
                pthread_rwlock_unlock(&rwlock);
                for (auto subscriptionPair : eventSubscriptionQueues)
                {
                    auto subscriptionName = subscriptionPair.first;
                    auto webhookUrl = eventSubscriptionWebhookMap[subscriptionName];
                    try
                    {
                        http::Request request{webhookUrl};
                        json json_body = message;
                        const std::string body = json_body.dump();
                        const auto response = request.send("POST", body, {{"Content-Type", "application/json"}});
                        if (response.status.code == http::Status::Ok)
                        {
                            std::cout << formatLogMessage("Message with Id: " + message.getMessageId() + " sent successully to webhookUrl: " + webhookUrl + " for subscription: " + subscriptionName) << std::endl;
                        }
                    }
                    catch (const std::exception &e)
                    {
                        std::cerr << "Message delivery failed for Message Id: " << message.getMessageId() << ", error: " << e.what() << '\n';
                    }
                }
            }
            std::vector<Message> fetchMessages(std::string subscriptionName)
            {

                std::vector<Message> res;
                if (subscriptionQueues.find(subscriptionName) == subscriptionQueues.end())
                {
                    throw std::runtime_error("Subscription with subscription name: " + subscriptionName + " does not exist!");
                }
                pthread_rwlock_rdlock(&rwlock);
                while (!subscriptionQueues[subscriptionName].empty())
                {
                    res.push_back(subscriptionQueues[subscriptionName].front());
                    subscriptionQueues[subscriptionName].pop();
                }
                pthread_rwlock_unlock(&rwlock);
                return res;
            }
        };

        std::unordered_map<std::string, TopicSubscriptionQueues *>
            topicsStore;

    public:
        bool publishTopic(std::string topicName)
        {
            std::cout << formatLogMessage("Publishing topic: " + topicName) << std::endl;
            if (topicsStore.find(topicName) == topicsStore.end())
            {
                topicsStore[topicName] = new TopicSubscriptionQueues();
                return true;
            }

            std::cout << formatLogMessage("Duplicate topic found: " + topicName) << std::endl;
            return false;
        }
        void subscribeToTopic(std::string topicName, std::string subscriptionName, const std::string type, std::string webhookUrl = "")
        {
            std::cout << formatLogMessage("Subscribing to topic: " + topicName + " with subscription name: " + subscriptionName + " of type: " + type) << std::endl;
            if (topicsStore.find(topicName) == topicsStore.end())
            {
                throw std::runtime_error("Topic with topic name: " + topicName + " does not exist!");
            }

            topicsStore[topicName]->addSubscription(subscriptionName, type, webhookUrl);
        }
        bool addMessage(std::string topicName, Message message)
        {
            std::cout << formatLogMessage("Message with Id: " + message.getMessageId() + " received for topic: " + topicName) << std::endl;
            if (topicsStore.find(topicName) == topicsStore.end())
            {
                throw std::runtime_error("Topic with topic name: " + topicName + " does not exist!");
            }

            topicsStore[topicName]->addMessage(message);
        }
        std::vector<Message> pullMessages(std::string topicName, std::string subscriptionName)
        {
            std::cout << formatLogMessage("Pulling messages for topic: " + topicName + " subscription: " + subscriptionName) << std::endl;
            if (topicsStore.find(topicName) == topicsStore.end())
            {
                throw std::runtime_error("Topic with topic name: " + topicName + " does not exist!");
            }

            return topicsStore[topicName]->fetchMessages(subscriptionName);
        }
        void initialize()
        {
            std::cout << "Initializing MessageProcessorService with existing topics and subscription queues" << std::endl;
            auto subscriptionRepository = DependencyInjectionContainer::resolve<SubscriptionRepository>();
            auto subscriptions = subscriptionRepository->GetAll();
            for (Subscription sub : subscriptions)
            {
                auto topicName = sub.getTopicName();
                auto subscriptionName = sub.getName();
                auto subscriptionType = sub.getSubscriptionType();
                auto webhookUrl = sub.getWebhookUrl();

                if (topicsStore.find(topicName) == topicsStore.end())
                {
                    publishTopic(topicName);
                }

                subscribeToTopic(topicName, subscriptionName, subscriptionType, webhookUrl);
            }
        }
    };
}