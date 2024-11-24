#include "service-handler.h"

namespace messagebrokerv1
{
    int ServiceHandler::serviceRegister(struct mg_connection *conn, void *cbdata)
    {
        const struct mg_request_info *request = mg_get_request_info(conn);
        char *content = new char[request->content_length + 1];
        int dlen = mg_read(conn, content, request->content_length);
        content[dlen] = '\0';

        try
        {
            auto payload = json::parse(content);
            auto service_register_request = payload.template get<ServiceRegisterRequestDTO>();
            auto serviceRepository = DependencyInjectionContainer::resolve<ServiceRepository>();
            auto service_id = serviceRepository->AddService(service_register_request);

            Service service(service_id, service_register_request.getName(), service_register_request.getScope());
            json response = service;

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
        auto message_processor = (MessageProcessorService *)cbdata;
        const struct mg_request_info *request = mg_get_request_info(conn);
        std::cout << "[" << request->request_method << "] " << request->request_uri << std::endl;
        char *content = new char[request->content_length + 1];
        int dlen = mg_read(conn, content, request->content_length);
        content[dlen] = '\0';

        try
        {
            auto payload = json::parse(content);
            auto topic_publish_request = payload.template get<TopicPublishRequestDTO>();
            auto topic_to_publish = topic_publish_request.getName();
            auto serviceRepository = DependencyInjectionContainer::resolve<ServiceRepository>();

            auto topic_exists = serviceRepository->CheckTopicExists(topic_to_publish);
            if (topic_exists)
            {
                auto status_response = new GenericResponseDTO(false, "Topic " + topic_to_publish + " already exists.");
                json response = *status_response;
                mg_send_http_error(conn, 400, response.dump().c_str());
                return 400;
            }

            struct mg_match_context mcx;
            mcx.case_sensitive = 0;
            std::cout << "Request Local URI: " << request->local_uri << std::endl;
            ptrdiff_t ret = mg_match((API_V1_PREFIX + SERVICE_TOPIC_PUBLISH_URL).c_str(), request->local_uri, &mcx);

            std::cout << mcx.num_matches << std::endl;

            char cservice_id[1024];
            memcpy(cservice_id, mcx.match[0].str, mcx.match[0].len);
            cservice_id[mcx.match[0].len] = 0;

            std::string service_id(cservice_id);

            std::cout << "Service Id: " << service_id << std::endl;

            std::string msg = "Topic published successfully";
            auto success = serviceRepository->AddTopicToService(service_id, topic_to_publish);
            success = message_processor->publishTopic(topic_to_publish);
            if (!success)
            {
                msg = "Duplicate Topic Found";
            }
            auto status_response = new GenericResponseDTO(success, msg);
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
    int ServiceHandler::sendMessage(struct mg_connection *conn, void *cbdata)
    {
        auto message_processor = (MessageProcessorService *)cbdata;
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
            ptrdiff_t ret = mg_match((API_V1_PREFIX + SERVICE_SEND_MESSAGE_URL).c_str(), request->local_uri, &mcx);

            char cservice_id[1024], ctopic_name[1024];
            memcpy(cservice_id, mcx.match[0].str, mcx.match[0].len);
            cservice_id[mcx.match[0].len] = 0;
            memcpy(ctopic_name, mcx.match[1].str, mcx.match[1].len);
            ctopic_name[mcx.match[1].len] = 0;

            std::string service_id(cservice_id);
            std::string topic_name(ctopic_name);

            Message message(message_send_request.getMessageId(), message_send_request.getPayload());
            auto success = message_processor->addMessage(topic_name, message);

            auto status_response = new GenericResponseDTO(success, "Message published successfully");
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
    int ServiceHandler::subscribeTopic(struct mg_connection *conn, void *cbdata)
    {
        auto message_processor = (MessageProcessorService *)cbdata;
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
            ptrdiff_t ret = mg_match((API_V1_PREFIX + SERVICE_TOPIC_SUBSCRIBE_URL).c_str(), request->local_uri, &mcx);

            char cservice_id[1024];
            memcpy(cservice_id, mcx.match[0].str, mcx.match[0].len);
            cservice_id[mcx.match[0].len] = 0;

            std::string service_id(cservice_id);

            auto subscriptionRepository = DependencyInjectionContainer::resolve<SubscriptionRepository>();
            auto subscriptionId = subscriptionRepository->AddSubscription(topic_subscription_request.getSubscriptionName(), topic_subscription_request.getTopicName(), topic_subscription_request.getSubscriptionType(), topic_subscription_request.getWebhookUrl());

            auto success = serviceRepository->AssociateSubscription(service_id, subscriptionId);
            message_processor->subscribeToTopic(topic_subscription_request.getTopicName(), topic_subscription_request.getSubscriptionName(), topic_subscription_request.getSubscriptionType(), topic_subscription_request.getWebhookUrl());

            auto subscription = subscriptionRepository->GetSubscription(subscriptionId);
            json response = subscription;

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
        auto message_processor = (MessageProcessorService *)cbdata;
        const struct mg_request_info *request = mg_get_request_info(conn);
        std::cout << "[" << request->request_method << "] " << request->request_uri << std::endl;

        try
        {
            struct mg_match_context mcx;
            mcx.case_sensitive = 0;
            ptrdiff_t ret = mg_match((API_V1_PREFIX + SERVICE_SUBSCRIPTION_PULL_MESSAGES_URL).c_str(), request->local_uri, &mcx);

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

            auto messages = message_processor->pullMessages(topic_name, sub_name);
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