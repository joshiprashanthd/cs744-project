#include "service-handler.h"
#include <dependency-injection-container.hpp>

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

            auto service_register_response = new ServiceRegisterResponseDTO(service_id, service_register_request.getName(), service_register_request.getScope());
            json response = *service_register_response;

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
} // namespace messagebrokerv1