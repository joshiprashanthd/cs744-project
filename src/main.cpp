#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
#include "civetweb.h"
#include <utils.h>
#include <router.h>
#include <service-handler.h>
#include <constants.h>
#include <dependency-injection-container.hpp>
#include <memory>
#include "mongocxx/instance.hpp"
#include <subscription-repository.h>
#include <message-processor.h>

#define PORT 3000
#define NUM_THREADS 10

void registerRoutes(messagebrokerv1::Router *router, messagebrokerv1::MessageProcessor *message_processor);
void configureServices();

int main(int argc, char **argv)
{
    mongocxx::instance instance;
    struct mg_context *ctx;
    struct mg_callbacks callbacks;
    messagebrokerv1::Router *router{nullptr};
    auto message_processor = new messagebrokerv1::MessageProcessor();

    auto listening_port = argc > 1 ? argv[1] : std::to_string(PORT);
    auto num_threads = argc > 2 ? argv[2] : std::to_string(NUM_THREADS);

    const char *options[] = {"listening_ports", listening_port.c_str(),
                             "num_threads", num_threads.c_str(),
                             nullptr};

    memset(&callbacks, 0, sizeof(callbacks));
    callbacks.log_message = messagebrokerv1::logMessage;

    configureServices();

    ctx = mg_start(&callbacks, 0, options);
    if (ctx == NULL)
    {
        fprintf(stderr, "Failed to start server!\n");
        return EXIT_FAILURE;
    }
    std::cout << messagebrokerv1::formatLogMessage("Server Started at port " + listening_port + "!") << std::endl;

    router = new messagebrokerv1::Router(ctx);
    registerRoutes(router, message_processor);

    std::cout << messagebrokerv1::formatLogMessage("Press any key to stop the server...") << std::endl;
    getchar();

    mg_stop(ctx);

    std::cout << messagebrokerv1::formatLogMessage("Server Stopped.") << std::endl;

    return EXIT_SUCCESS;
}

void configureServices()
{
    try
    {
        messagebrokerv1::DependencyInjectionContainer::registerObject(std::make_shared<messagebrokerv1::ServiceRepository>(MONGO_URI, MONGO_DATABASE_NAME));
        messagebrokerv1::DependencyInjectionContainer::registerObject(std::make_shared<messagebrokerv1::SubscriptionRepository>(MONGO_URI, MONGO_DATABASE_NAME));
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void registerRoutes(messagebrokerv1::Router *router, messagebrokerv1::MessageProcessor *message_processor)
{
    router->registerRoute("/services/register", messagebrokerv1::ServiceHandler::serviceRegister, message_processor);
    router->registerRoute("/services/*/topics/publish", messagebrokerv1::ServiceHandler::publishTopic, message_processor);
    router->registerRoute("/services/*/topics/*/send", messagebrokerv1::ServiceHandler::addMessage, message_processor);
    router->registerRoute("/services/*/topics/*/subscriptions/subscribe", messagebrokerv1::ServiceHandler::subscribeTopic, message_processor);
    router->registerRoute("/services/*/topics/*/subscriptions/*/messages", messagebrokerv1::ServiceHandler::fetchMessages, message_processor);
}