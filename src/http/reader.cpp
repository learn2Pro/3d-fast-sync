#include <iostream>
#include "httplib.h"
#define CPPHTTPLIB_OPENSSL_SUPPORT

using namespace std;

int main()
{
    try
    {
        // HTTP
        httplib::Client cli("http://baidu.com");

        // send a get request
        auto response = cli.Get("/");
        std::cout << "<<<<<<<<<<<<<<<start request<<<<<<<<<<<<<<<" << endl;
        // print the result
        std::cout << "status:" << response->status
                  << endl
                  << "body:" << endl
                  << response->body << '\n';
        std::cout << "<<<<<<<<<<<<<<<end request<<<<<<<<<<<<<<<" << endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Request failed, error: " << e.what() << '\n';
    }
}
