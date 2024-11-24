#include "subscription-repository.h"

namespace messagebrokerv1
{
    SubscriptionRepository::SubscriptionRepository(std::string uri, std::string databaseName)
        : BaseRepository(uri, databaseName), collectionName("subscriptions")
    {
    }
    std::vector<Subscription> SubscriptionRepository::GetAll()
    {
        std::vector<Subscription> subscriptions;
        auto collection = db[collectionName];

        auto cursor = collection.find({});
        for (const auto &doc : cursor)
        {
            auto json_string = bsoncxx::to_json(doc);
            nlohmann::json json_obj = nlohmann::json::parse(json_string);
            auto subscription = json_obj.get<Subscription>();
            subscriptions.push_back(subscription);
        }

        return subscriptions;
    }
    Subscription SubscriptionRepository::GetSubscription(std::string subscriptionId)
    {
        auto builder = bsoncxx::builder::stream::document{};
        auto collection = db[collectionName];
        bsoncxx::oid document_id{subscriptionId};

        builder << "_id" << document_id;

        auto maybe_result = collection.find_one(builder.view());
        if (!maybe_result)
        {
            throw std::runtime_error("Subscription with ID " + subscriptionId + " not found");
        }
        auto doc = maybe_result->view();

        // Convert BSON to JSON string and use nlohmann::json
        auto json_string = bsoncxx::to_json(doc);
        nlohmann::json json_obj = nlohmann::json::parse(json_string);

        // Deserialize into a Subscription object
        Subscription subscription = json_obj.get<Subscription>();

        return subscription;
    }
    bool SubscriptionRepository::CheckSubscriptionExists(std::string subscriptionName)
    {
        auto builder = bsoncxx::builder::stream::document{};
        auto collection = db[collectionName];

        builder << "name" << subscriptionName;

        auto maybe_result = collection.find_one(builder.view());
        return maybe_result ? true : false;
    }
    std::string SubscriptionRepository::AddSubscription(std::string name, std::string topicName, const std::string type, std::string webhookUrl = "")
    {
        auto builder = bsoncxx::builder::stream::document{};
        auto collection = db[collectionName];

        bsoncxx::document::value doc_to_add =
            builder << "name" << name
                    << "topicName" << topicName
                    << "type" << type
                    << "webhookUrl" << webhookUrl
                    << bsoncxx::builder::stream::finalize;

        auto maybe_result = collection.insert_one(doc_to_add.view());

        return maybe_result ? maybe_result->inserted_id().get_oid().value.to_string() : nullptr;
    }
} // namespace messagebrokerv1
