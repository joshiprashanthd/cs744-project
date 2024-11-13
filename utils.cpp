#include <iostream>

#include "utils.h"

const response *read_content(struct mg_connection *conn) {
    const struct mg_response_info *ri = mg_get_response_info(conn);

    if (ri == NULL) return NULL;
    std::cout << "YES" << std::endl;

    char *content = (char *)malloc(ri->content_length + 1);
    int bytes_read = mg_read(conn, content, ri->content_length);
    content[bytes_read] = '\0';

    struct response *res = (struct response *)malloc(sizeof(struct response));
    res->info = (struct mg_response_info *)ri;
    res->content = content;

    return res;
}

Json::Json() { obj = cJSON_CreateObject(); }

Json::Json(cJSON *o) { obj = o; }

void Json::add(const std::string key, const std::string value) {
    cJSON_AddStringToObject(obj, key.c_str(), value.c_str());
}

Json *Json::parse(const std::string &json) {
    return new Json(cJSON_Parse(json.c_str()));
}

std::string Json::stringify() { return cJSON_PrintUnformatted(obj); }

Client::Client() {
    char errbuf[256] = {0};

    conn = mg_connect_client("localhost", 8080, 0, errbuf, sizeof(errbuf));

    if (conn == NULL) {
        fprintf(stderr, "Cannot connect client: %s\n", errbuf);
        return;
    }

    std::cout << "Client connected\n" << std::endl;
}

Client::~Client() { mg_close_connection(conn); }

const struct response *Client::send_get_request(const std::string &path,
                                                const std::string &content) {
    if (conn == NULL) {
        fprintf(stderr, "Client is not connected\n");
        return NULL;
    }

    mg_printf(conn,
              "POST %s"
              " HTTP/1.1\r\n"
              "Host: localhost:8080\r\n"
              "Content-Type: application/json\r\n"
              "Content-Length: %ld\r\n\r\n"
              "%s\r\n",
              path.c_str(), content.size(), content.c_str());

    // mg_write(conn, content.c_str(), content.size());
    int ret = mg_get_response(conn, NULL, 0, 10000);
    if (ret < 0) return NULL;

    return read_content(conn);
}

Server::Server() { callbacks = {0}; }

Server::~Server() {
    if (ctx) mg_stop(ctx);
}

void Server::startServer() {
    const char *options[] = {"document_root", ".", "listening_ports", "8080",
                             NULL};

    ctx = mg_start(&callbacks, 0, options);

    if (ctx == NULL) {
        std::cout << "Failed to start server" << std::endl;
        exit(1);
    }

    // print server ip and port
    std::cout << "Server started at http://localhost:"
              << mg_get_option(ctx, "listening_ports") << std::endl;
}

void Server::addHandler(const std::string &uri, mg_request_handler handler) {
    mg_set_request_handler(ctx, uri.c_str(), handler, this->ctx);
}
