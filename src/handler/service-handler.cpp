#include "service-handler.h"
#include "subscription-repository.h"
#include "message-processor.h"
#include <dependency-injection-container.hpp>

namespace messagebrokerv1
{
    int ServiceHandler::serviceRegister(struct mg_connection *conn, void *cbdata)
    {
        const struct mg_request_info *request = mg_get_request_info(conn);
        std::cout << "[" << request->request_method << "] " << request->request_uri << std::endl;
        char *content = new char[request->content_length + 1];
        int dlen = mg_read(conn, content, request->content_length);
        content[dlen] = '\0';
        try
        {
            auto payload = json::parse(content);
            auto service_register_request = payload.template get<ServiceRegisterRequestDTO>();
            auto serviceRepository = DependencyInjectionContainer::resolve<ServiceRepository>();
            auto service_id = serviceRepository->AddService(service_register_request);

            auto created_service = new Service(service_id, service_register_request.getName(), service_register_request.getScope());
            json response = *created_service;

            return sendJsonResponse(conn, response) ? 201 : 500;
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

    int ServiceHandler::publishTopic(struct mg_connection *conn, void *cbdata)
    {
        auto message_processor = (MessageProcessor *)cbdata;
        const struct mg_request_info *request = mg_get_request_info(conn);
        std::cout << "[" << request->request_method << "] " << request->request_uri << std::endl;
        char *content = new char[request->content_length + 1];
        int dlen = mg_read(conn, content, request->content_length);
        content[dlen] = '\0';

        try
        {
            auto payload = json::parse(content);
            auto topic_publish_request = payload.template get<TopicPublishRequestDTO>();
            auto serviceRepository = DependencyInjectionContainer::resolve<ServiceRepository>();

            struct mg_match_context mcx;
            mcx.case_sensitive = 0;
            ptrdiff_t ret = mg_match("/api/v1/services/*/topics", request->local_uri, &mcx);

            char cservice_id[1024];
            memcpy(cservice_id, mcx.match[0].str, mcx.match[0].len);
            cservice_id[mcx.match[0].len] = 0;

            std::string service_id(cservice_id);

            auto success = serviceRepository->AddTopic(service_id, topic_publish_request.getName());
            message_processor->addTopic(topic_publish_request.getName());
            auto status_response = new GenericResponseDTO(success);
            json response = *status_response;

            return sendJsonResponse(conn, response) ? 201 : 500;
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

    int ServiceHandler::subscribeTopic(struct mg_connection *conn, void *cbdata)
    {
        auto message_processor = (MessageProcessor *)cbdata;
        const struct mg_request_info *request = mg_get_request_info(conn);
        std::cout << "[" << request->request_method << "] " << request->request_uri << std::endl;
        char *content = new char[request->content_length + 1];
        int dlen = mg_read(conn, content, request->content_length);
        content[dlen] = '\0';

        try
        {
            auto payload = json::parse(content);
            auto topic_subscription_request = payload.template get<TopicSubscriptionRequestDTO>();
            auto serviceRepository = DependencyInjectionContainer::resolve<ServiceRepository>();

            struct mg_match_context mcx;
            mcx.case_sensitive = 0;
            ptrdiff_t ret = mg_match("/api/v1/services/*/topics/*/subscriptions/subscribe", request->local_uri, &mcx);

            char cservice_id[1024], ctopic_name[1024];
            memcpy(cservice_id, mcx.match[0].str, mcx.match[0].len);
            cservice_id[mcx.match[0].len] = 0;
            memcpy(ctopic_name, mcx.match[1].str, mcx.match[1].len);
            ctopic_name[mcx.match[1].len] = 0;

            std::string service_id(cservice_id);
            std::string topic_name(ctopic_name);

            auto subscriptionRepository = DependencyInjectionContainer::resolve<SubscriptionRepository>();
            auto subscriptionId = subscriptionRepository->addSubscription(topic_subscription_request.getName(), topic_name);

            auto success = serviceRepository->AddSubscription(service_id, subscriptionId);
            message_processor->addSubscription(topic_name, topic_subscription_request.getName());
            auto status_response = new GenericResponseDTO(success);
            json response = *status_response;

            return sendJsonResponse(conn, response) ? 201 : 500;
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

    int ServiceHandler::addMessage(struct mg_connection *conn, void *cbdata)
    {
        auto message_processor = (MessageProcessor *)cbdata;
        const struct mg_request_info *request = mg_get_request_info(conn);
        std::cout << "[" << request->request_method << "] " << request->request_uri << std::endl;
        char *content = new char[request->content_length + 1];
        int dlen = mg_read(conn, content, request->content_length);
        content[dlen] = '\0';

        try
        {
            auto payload = json::parse(content);
            auto message_send_request = payload.template get<MessageSendDTO>();
            std::cout << formatLogMessage("Message Received: " + payload.dump()) << std::endl;

            struct mg_match_context mcx;
            mcx.case_sensitive = 0;
            ptrdiff_t ret = mg_match("/api/v1/services/*/topics/*/send", request->local_uri, &mcx);

            char cservice_id[1024], ctopic_name[1024];
            memcpy(cservice_id, mcx.match[0].str, mcx.match[0].len);
            cservice_id[mcx.match[0].len] = 0;
            memcpy(ctopic_name, mcx.match[1].str, mcx.match[1].len);
            ctopic_name[mcx.match[1].len] = 0;

            std::string service_id(cservice_id);
            std::string topic_name(ctopic_name);

            Message message(message_send_request.getMessageId(), message_send_request.getPayload());
            auto success = message_processor->addMessage(topic_name, message);

            auto status_response = new GenericResponseDTO(success);
            json response = message;

            return sendJsonResponse(conn, response) ? 201 : 500;
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

    int ServiceHandler::fetchMessages(struct mg_connection *conn, void *cbdata)
    {
        auto message_processor = (MessageProcessor *)cbdata;
        const struct mg_request_info *request = mg_get_request_info(conn);
        std::cout << "[" << request->request_method << "] " << request->request_uri << std::endl;

        try
        {
            struct mg_match_context mcx;
            mcx.case_sensitive = 0;
            ptrdiff_t ret = mg_match("/api/v1/services/*/topics/*/subscriptions/*/messages", request->local_uri, &mcx);

            char cservice_id[1024], ctopic_name[1024], csub_name[1024];
            memcpy(cservice_id, mcx.match[0].str, mcx.match[0].len);
            cservice_id[mcx.match[0].len] = 0;
            memcpy(ctopic_name, mcx.match[1].str, mcx.match[1].len);
            ctopic_name[mcx.match[1].len] = 0;
            memcpy(csub_name, mcx.match[2].str, mcx.match[2].len);
            csub_name[mcx.match[2].len] = 0;

            std::string service_id(cservice_id);
            std::string topic_name(ctopic_name);
            std::string sub_name(csub_name);

            auto messages = message_processor->fetch(topic_name, sub_name);
            json response(messages);
            return sendJsonResponse(conn, response)
                       ? 200
                       : 500;
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
} // namespace messagebrokerv1