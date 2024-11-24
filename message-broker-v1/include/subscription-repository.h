#pragma once

#include "base-repository.h"
#include "dtos.h"
#include "models.h"

namespace messagebrokerv1
{
    class SubscriptionRepository : public BaseRepository
    {
    private:
        std::string collectionName;

    public:
        SubscriptionRepository(std::string uri, std::string databaseName);
        std::vector<Subscription> GetAll();
        Subscription GetSubscription(std::string subscriptionId);
        bool CheckSubscriptionExists(std::string subscriptionName);
        std::string AddSubscription(std::string name, std::string topicName, const std::string type, std::string webhookUrl);
    };
}