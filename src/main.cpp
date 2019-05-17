#include <curl/curl.h>
#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

#include "imgup.hpp"
#include "request.hpp"

int main(int argc, char **argv)
{
    if(argc < 2 || argc >= 3)
    {
        std::cout << "Proper usage: imgup <image filepath>";
        return 1;
    }

    std::string url{};
    curl_global_init(CURL_GLOBAL_ALL);

    try
    {
        url = imgup::upload_imgur(std::string(argv[1]));
    }
    catch(const nlohmann::json::exception &e)
    {
        std::cerr << e.what() << "\n\n";
        curl_global_cleanup();
        return 1;
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << "\n\n";
        curl_global_cleanup();
        return 1;
    }
    
    std::cout << "URL:\n" << url << "\n\n";

    curl_global_cleanup();
    return 0;
}