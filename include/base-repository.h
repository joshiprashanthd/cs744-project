#pragma once

#include <string>
#include "bsoncxx/builder/stream/document.hpp"
#include "bsoncxx/json.hpp"
#include "bsoncxx/oid.hpp"
#include "mongocxx/client.hpp"
#include "mongocxx/database.hpp"
#include "mongocxx/uri.hpp"

namespace messagebrokerv1
{
    class BaseRepository
    {
    private:
        mongocxx::client client;

    protected:
        mongocxx::database db;

    public:
        BaseRepository(const std::string uri, const std::string database_name);
    };
}