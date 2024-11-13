#include <stdio.h>

#include <iostream>
#include <string>

using namespace std;

#include "utils.h"

int publisher_register(struct mg_connection *conn, void *cbdata) {
    const struct mg_request_info *request_info = mg_get_request_info(conn);
    cout << "Publisher Request from " << request_info->remote_addr << endl;

    std::cout << "Content Length = " << request_info->content_length << endl;
    char *content = new char[request_info->content_length + 1];
    int bytes_read = mg_read(conn, content, request_info->content_length);
    content[bytes_read] = '\0';

    std::cout << "Content in request = " << content << endl;

    auto j = new Json();
    j->add("id", "123094");

    mg_printf(conn,
              "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: "
              "%ld\r\n\r\n%s",
              j->stringify().size(), j->stringify().c_str());
    return 1;
}

int subscriber_register(struct mg_connection *conn, void *cbdata) {
    const struct mg_request_info *request_info = mg_get_request_info(conn);
    cout << "Subscriber Request from " << request_info->remote_addr << endl;

    const struct response *res = read_content(conn);
    std::cout << "Content in request = " << res << endl;

    auto j = new Json();
    j->add("id", "123094");

    mg_printf(conn,
              "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: "
              "%ld\r\n\r\n%s",
              j->stringify().size(), j->stringify().c_str());

    return 1;
}

int main() {
    auto server = new Server();
    server->startServer();
    server->addHandler("/", publisher_register);
    server->addHandler("/publisher/register", publisher_register);
    server->addHandler("/subscriber/register", subscriber_register);
    getchar();
    return 0;
}
