#include "service-repository.h"

namespace messagebrokerv1
{
    ServiceRepository::ServiceRepository(std::string uri, std::string databaseName)
        : BaseRepository(uri, databaseName), collectionName("services")
    {
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

        return maybe_result ? maybe_result->inserted_id().get_oid().value.to_string() : NULL;
    }
    bool ServiceRepository::AddTopic(std::string serviceId, std::string topic)
    {
        try
        {
            auto builder = bsoncxx::builder::stream::document{};
            mongocxx::collection collection = db[collectionName];
            bsoncxx::oid document_id(serviceId);

            bsoncxx::document::value query_doc =
                builder << "_id" << document_id
                        << bsoncxx::builder::stream::finalize;

            bsoncxx::document::value update_doc =
                builder << "$push" << bsoncxx::builder::stream::open_document
                        << "topics" << topic
                        << bsoncxx::builder::stream::close_document
                        << bsoncxx::builder::stream::finalize;

            auto result = collection.update_one(query_doc.view(), update_doc.view());

            if (result)
            {
                return result->modified_count() == 1;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }

        return false;
    }
    bool ServiceRepository::AddSubscription(std::string serviceId, std::string subscriptionId)
    {
        try
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

            if (result)
            {
                return result->modified_count() == 1;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }

        return false;
    }

} // namespace name
