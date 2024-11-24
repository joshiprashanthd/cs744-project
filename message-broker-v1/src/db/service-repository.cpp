#include "service-repository.h"

namespace messagebrokerv1
{
    ServiceRepository::ServiceRepository(std::string uri, std::string databaseName)
        : BaseRepository(uri, databaseName), collectionName("services")
    {
    }
    bool ServiceRepository::CheckServiceExists(std::string serviceName, std::string serviceScope)
    {
        auto collection = db[collectionName];
        auto builder = bsoncxx::builder::stream::document{};

        builder << "scope" << serviceScope
                << "name" << serviceName;

        auto maybe_result = collection.find_one(builder.view());

        return maybe_result ? true : false;
    }
    std::string ServiceRepository::AddService(ServiceRegisterRequestDTO request)
    {
        mongocxx::collection collection = db[collectionName];
        auto builder = bsoncxx::builder::stream::document{};

        bsoncxx::document::value doc_to_add =
            builder << "name" << request.getName()
                    << "scope" << request.getScope()
                    << "topics" << bsoncxx::builder::stream::open_array
                    << bsoncxx::builder::stream::close_array
                    << "subscriptions" << bsoncxx::builder::stream::open_array
                    << bsoncxx::builder::stream::close_array
                    << bsoncxx::builder::stream::finalize;

        bsoncxx::stdx::optional<mongocxx::result::insert_one> maybe_result = collection.insert_one(doc_to_add.view());

        return maybe_result ? maybe_result->inserted_id().get_oid().value.to_string() : nullptr;
    }
    bool ServiceRepository::CheckTopicExists(std::string topicName)
    {
        auto collection = db[collectionName];
        auto cursor = collection.find({});

        for (const auto &doc : cursor)
        {
            if (doc["topics"] && doc["topics"].type() == bsoncxx::type::k_array)
            {
                bsoncxx::array::view topics{doc["topics"].get_array().value};

                for (const auto &topic : topics)
                {
                    if (topic.type() == bsoncxx::type::k_utf8 && std::string(topic.get_string().value) == topicName)
                    {
                        return true;
                    }
                }
            }
        }

        return false;
    }
    bool ServiceRepository::AddTopicToService(std::string serviceId, std::string topicName)
    {
        auto builder = bsoncxx::builder::stream::document{};
        auto collection = db[collectionName];
        bsoncxx::oid document_id{serviceId};

        bsoncxx::document::value query_doc =
            builder << "_id" << document_id
                    << bsoncxx::builder::stream::finalize;

        bsoncxx::document::value update_doc =
            builder << "$push" << bsoncxx::builder::stream::open_document
                    << "topics" << topicName
                    << bsoncxx::builder::stream::close_document
                    << bsoncxx::builder::stream::finalize;

        auto result = collection.update_one(query_doc.view(), update_doc.view());

        return result ? result->modified_count() == 1 : false;
    }
    bool ServiceRepository::AssociateSubscription(std::string serviceId, std::string subscriptionId)
    {
        auto builder = bsoncxx::builder::stream::document{};
        mongocxx::collection collection = db[collectionName];
        bsoncxx::oid document_id(serviceId);

        bsoncxx::document::value query_doc =
            builder << "_id" << document_id
                    << bsoncxx::builder::stream::finalize;

        bsoncxx::document::value update_doc =
            builder << "$push" << bsoncxx::builder::stream::open_document
                    << "subscriptions" << subscriptionId
                    << bsoncxx::builder::stream::close_document
                    << bsoncxx::builder::stream::finalize;

        auto result = collection.update_one(query_doc.view(), update_doc.view());

        return result ? result->modified_count() == 1 : false;
    }
} // namespace name
