#pragma once

#include "base-repository.h"
#include "dtos.h"
#include "utils.h"

namespace messagebrokerv1
{
    class ServiceRepository : public BaseRepository
    {
    private:
        std::string collectionName;

    public:
        ServiceRepository(std::string uri, std::string databaseName);
        bool CheckServiceExists(std::string serviceName, std::string serviceScope);
        std::string AddService(ServiceRegisterRequestDTO request);
        bool CheckTopicExists(std::string topicName);
        bool AddTopicToService(std::string serviceId, std::string topicName);
        bool AssociateSubscription(std::string serviceId, std::string subscriptionId);
    };
}