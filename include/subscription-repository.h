#pragma once

#include "base-repository.h"
#include "models.h"
#include "utils.h"

namespace messagebrokerv1
{
    class SubscriptionRepository : public BaseRepository
    {
    private:
        std::string collectionName;

    public:
        SubscriptionRepository(std::string uri, std::string databaseName);
        std::string addSubscription(std::string subscriptionName, std::string topicName);
    };
}