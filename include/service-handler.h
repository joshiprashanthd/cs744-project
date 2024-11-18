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
    };
}