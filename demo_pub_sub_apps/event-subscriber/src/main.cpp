#include <iostream>
#include "civetweb.h"
#include "dtos.h"

#define PORT 4000
#define NUM_THREADS 10

bool sendJsonResponse(struct mg_connection *conn, json response)
{
    std::string response_str = response.dump();
    size_t content_length = response_str.size();

    mg_send_http_ok(conn, "application/json; charset=utf-8", content_length + 1);

    return mg_write(conn, response_str.c_str(), content_length) > 0;
}
static int messages_event_handler(struct mg_connection *conn, void *cbdata)
{
    const struct mg_request_info *request = mg_get_request_info(conn);
    std::cout << "[" << request->request_method << "] " << request->request_uri << std::endl;
    char *content = new char[request->content_length + 1];
    int dlen = mg_read(conn, content, request->content_length);
    content[dlen] = '\0';

    std::cout << "New Message Received" << std::endl;

    try
    {
        auto payload = json::parse(content);
        std::cout << "Message: " << payload.dump() << std::endl;

        auto status_response = new subscriberv1::GenericResponseDTO(true, "Message Received Successfully");
        json response = *status_response;

        return sendJsonResponse(conn, response) ? 200 : 500;
    }
    catch (const json::exception &e)
    {
        std::cerr << "deserialization failed: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 500;
}

int main(int argc, char **argv)
{
    struct mg_context *ctx;
    struct mg_callbacks callbacks;
    auto listening_port = argc > 1 ? argv[1] : std::to_string(PORT);
    auto num_threads = argc > 2 ? argv[2] : std::to_string(NUM_THREADS);

    const char *options[] = {"listening_ports", listening_port.c_str(),
                             "num_threads", num_threads.c_str(),
                             nullptr};

    memset(&callbacks, 0, sizeof(callbacks));

    ctx = mg_start(&callbacks, 0, options);
    if (ctx == NULL)
    {
        fprintf(stderr, "Failed to start server!\n");
        return EXIT_FAILURE;
    }
    std::cout << "Event Subscriber Started at port: " << listening_port << std::endl;

    mg_set_request_handler(ctx, "/api/v1/events", messages_event_handler, 0);

    std::cout << "Listening for messages on POST /api/v1/events route" << std::endl;

    getchar();

    mg_stop(ctx);

    return EXIT_SUCCESS;
}