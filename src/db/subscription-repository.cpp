#include "subscription-repository.h"

namespace messagebrokerv1
{
    SubscriptionRepository::SubscriptionRepository(std::string uri, std::string databaseName)
        : BaseRepository(uri, databaseName), collectionName("subscriptions")
    {
    }

    std::string SubscriptionRepository::addSubscription(std::string subscriptionName, std::string topicName)
    {
        mongocxx::collection collection = db[collectionName];
        auto builder = bsoncxx::builder::stream::document{};

        bsoncxx::document::value doc_to_add =
            builder << "name" << subscriptionName
                    << "topicName" << topicName
                    << bsoncxx::builder::stream::finalize;

        bsoncxx::stdx::optional<mongocxx::result::insert_one> maybe_result = collection.insert_one(doc_to_add.view());

        return maybe_result ? maybe_result->inserted_id().get_oid().value.to_string() : NULL;
    }

} // namespace messagebroker1
