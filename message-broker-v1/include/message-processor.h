#pragma once

#include <unordered_map>
#include <queue>
#include <vector>
#include <iostream>
#include "models.h"
#include "service-repository.h"

namespace messagebrokerv1
{
    class TopicSubscriptionQueues
    {
        std::unordered_map<std::string, std::queue<Message>> subscriptionQueues;

    public:
        bool addSubscription(std::string subscription)
        {
            std::queue<Message> subQueue;
            subscriptionQueues[subscription] = subQueue;
        }
        void addMessage(Message message)
        {
            for (auto subscriptionPair : subscriptionQueues)
            {
                subscriptionQueues[subscriptionPair.first].push(message);
            }
        }
        std::vector<Message> fetchMessages(std::string subscription)
        {
            std::vector<Message> res;
            if (subscriptionQueues.find(subscription) != subscriptionQueues.end())
            {
                while (!subscriptionQueues[subscription].empty())
                {
                    res.push_back(subscriptionQueues[subscription].front());
                    subscriptionQueues[subscription].pop();
                }
            }
            else
            {
                std::cout << formatLogMessage("Did not find Subscription") << std::endl;
            }
            return res;
        }
    };

    class MessageProcessor
    {
        std::unordered_map<std::string, TopicSubscriptionQueues *> topics;

    public:
        bool addTopic(std::string topic)
        {
            std::cout << formatLogMessage("Adding topic " + topic + " to Message Processor Queues!") << std::endl;
            topics[topic] = new TopicSubscriptionQueues();
            return true;
        }
        bool addSubscription(std::string topic, std::string subscription)
        {
            std::cout << formatLogMessage("Adding subscription " + subscription + " to topic " + topic + " Queue in Message Processor!") << std::endl;
            if (topics.find(topic) != topics.end())
            {
                std::cout << formatLogMessage("Found topic Subscription manager!") << std::endl;
                topics[topic]->addSubscription(subscription);
                return true;
            }
            std::cout << formatLogMessage("Did not find Topic Subscription manager! Returning false") << std::endl;
            return false;
        }
        bool addMessage(std::string topic, Message message)
        {
            std::cout << formatLogMessage("Adding message id " + message.getMessageId() + " to topic queues " + topic) << std::endl;
            if (topics.find(topic) != topics.end())
            {
                std::cout << formatLogMessage("Found topic Subscription manager!") << std::endl;
                topics[topic]->addMessage(message);
                return true;
            }
            std::cout << formatLogMessage("Did not find Topic Subscription manager! Returning false") << std::endl;
            return false;
        }
        std::vector<Message> fetch(std::string topic, std::string subscription)
        {
            std::cout << formatLogMessage("Fetching messages for topic " + topic + " and subscription " + subscription) << std::endl;
            if (topics.find(topic) != topics.end())
            {
                std::cout << formatLogMessage("Found topic Subscription manager!") << std::endl;
                return topics[topic]->fetchMessages(subscription);
            }
            std::cout << formatLogMessage("Did not find Topic Subscription manager! Returning false") << std::endl;
            return {};
        }
    };
} // namespace messagebrokerv1
