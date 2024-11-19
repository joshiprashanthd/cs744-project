#include "router.h"
#include "service-handler.h"

namespace messagebrokerv1
{
    Router::Router(struct mg_context *ctx) : ctx(ctx) {}
    void Router::registerRoute(const std::string &route, mg_request_handler handler, void *cbdata)
    {
        std::string route_path = API_V1_PREFIX + route;
        mg_set_request_handler(this->ctx, route_path.c_str(), handler, cbdata);
    }
}