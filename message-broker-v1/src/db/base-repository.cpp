#include "base-repository.h"

namespace messagebrokerv1
{
    BaseRepository::BaseRepository(const std::string uri, const std::string database_name) : client(mongocxx::client(mongocxx::uri(uri))),
                                                                                             db(client[database_name])
    {
    }
} // namespace name
