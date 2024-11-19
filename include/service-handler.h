#pragma once

#include "civetweb.h"
#include "service-repository.h"

namespace messagebrokerv1
{
    class ServiceHandler
    {
    public:
        ServiceHandler() = default;
        static int serviceRegister(struct mg_connection *conn, void *cbdata);
        static int publishTopic(struct mg_connection *conn, void *cbdata);
        static int subscribeTopic(struct mg_connection *conn, void *cbdata);
        static int addMessage(struct mg_connection *conn, void *cbdata);
        static int fetchMessages(struct mg_connection *conn, void *cbdata);
    };
}