#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <vector>

#include "cJSON/cJSON.h"
#include "civetweb/include/civetweb.h"

struct response {
    struct mg_response_info *info;
    char *content;
};

const response *read_content(struct mg_connection *conn);

class Json {
   private:
    cJSON *obj;

   public:
    Json();
    Json(cJSON *);

    void add(const std::string key, const std::string value);
    std::string stringify();
    static Json *parse(const std::string &json);
};

class Client {
   private:
    struct mg_connection *conn;

   public:
    Client();
    ~Client();

    const struct response *send_get_request(const std::string &path,
                                            const std::string &content);
};

class Server {
   private:
    struct mg_context *ctx;
    struct mg_callbacks callbacks;

   public:
    Server();
    ~Server();

    void startServer();
    void addHandler(const std::string &uri, mg_request_handler handler);
};

#endif
