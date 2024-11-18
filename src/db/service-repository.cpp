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
                    << bsoncxx::builder::stream::finalize;

        bsoncxx::stdx::optional<mongocxx::result::insert_one> maybe_result = collection.insert_one(doc_to_add.view());

        return maybe_result ? maybe_result->inserted_id().get_oid().value.to_string() : NULL;
    }

} // namespace name
