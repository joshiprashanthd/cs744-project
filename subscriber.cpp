#include <iostream>

#include "utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    auto j = new Json();
    j->add("subscriber event", "some value");

    auto client = new Client();
    auto res = client->send_get_request("/subscriber/register", j->stringify());

    cout << "Content: " << res->content << endl;

    return EXIT_SUCCESS;
}
