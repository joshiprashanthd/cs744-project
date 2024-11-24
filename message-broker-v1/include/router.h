#pragma once

#include <string>
#include "civetweb.h"
#include "constants.h"

namespace messagebrokerv1
{
    class Router
    {
        struct mg_context *ctx{nullptr};

    public:
        Router(struct mg_context *ctx);
        void registerRoute(const std::string &route, mg_request_handler handler, void *args);
    };
}