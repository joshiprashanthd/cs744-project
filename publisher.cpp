#include <iostream>

using namespace std;

#include "utils.h"

int main(int argc, char *argv[]) {
    auto j = new Json();
    j->add("publisher event", "some value");

    auto client = new Client();
    auto res = client->send_get_request("/publisher/register", j->stringify());

    cout << "Content: " << res->content << endl;

    return EXIT_SUCCESS;
}
