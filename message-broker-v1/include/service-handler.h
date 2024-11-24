#pragma once

#include <dependency-injection-container.hpp>
#include "message-processor-service.h"
#include "routes.h"
#include "civetweb.h"
#include "service-repository.h"
#include "constants.h"

namespace messagebrokerv1
{
    class ServiceHandler
    {
    public:
        ServiceHandler() = default;
        static int serviceRegister(struct mg_connection *conn, void *cbdata);
        static int publishTopic(struct mg_connection *conn, void *cbdata);
        static int sendMessage(struct mg_connection *conn, void *cbdata);
        static int subscribeTopic(struct mg_connection *conn, void *cbdata);
        static int fetchMessages(struct mg_connection *conn, void *cbdata);
    };
}