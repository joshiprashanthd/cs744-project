#pragma once

#include "base-repository.h"
#include "models.h"
#include "utils.h"

namespace messagebrokerv1
{
    class ServiceRepository : public BaseRepository
    {
    private:
        std::string collectionName;

    public:
        ServiceRepository(std::string uri, std::string databaseName);
        std::string AddService(ServiceRegisterRequestDTO request);
        bool AddTopic(std::string serviceId, std::string topic);
        bool AddSubscription(std::string serviceId, std::string subscriptionId);
    };
}