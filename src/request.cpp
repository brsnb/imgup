#include "request.hpp"

#include <iostream>

namespace request
{

Request::Request()
    : curl{std::unique_ptr<CURL, CURL_cleanup>(curl_easy_init())}
{
}

void Request::clear()
{
    curl = std::unique_ptr<CURL, CURL_cleanup>(curl_easy_init());
}

size_t Request::write_callback(char *content, size_t size, size_t nmemb, void *user_data)
{
    ((std::string*)user_data)->append((char*)content, size * nmemb);
    return size * nmemb;
}

std::string Request::http_post(const std::string &url,
                               const std::string &post_fields,
                               const std::vector<std::string> &headers)
{
    // debug
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    // url
    curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());

    // user agent
    curl_easy_setopt(curl.get(), CURLOPT_USERAGENT, "curl 7.64.1 (x86_64-pc-linux-gnu) libcurl/7.64.1");

    // post fields
    curl_easy_setopt(curl.get(), CURLOPT_POSTFIELDS, post_fields.c_str());

    // headers
    struct curl_slist *slist = NULL;
    for(auto header : headers)
    {
        slist = curl_slist_append(slist, header.c_str());
    }
    curl_easy_setopt(curl.get(), CURLOPT_HTTPHEADER, slist);

    // callback
    std::string callback;
    curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA,     &callback);

    CURLcode res = curl_easy_perform(curl.get());

    if(res != CURLE_OK)
    {
        std::cout << "Something happened in http_post\n";
        throw Request_exception();
    }

    return callback;
}

std::string Request::http_image_post(const std::string &url,
                                     const std::vector<std::string> &headers,
                                     const std::string &filename)
{
    // debug
    //curl_easy_setopt(curl.get(), CURLOPT_VERBOSE, 1L);

    // headers
    struct curl_slist *slist = NULL;
    for(auto header : headers)
    {
        slist = curl_slist_append(slist, header.c_str());
    }
    curl_easy_setopt(curl.get(), CURLOPT_HTTPHEADER, slist);

    // url
    curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());

    // user agent
    curl_easy_setopt(curl.get(), CURLOPT_USERAGENT, "curl 7.64.1 (x86_64-pc-linux-gnu) libcurl/7.64.1");

    // callback
    std::string callback;
    curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA,     &callback);

    // image data
    curl_mime *multipart = curl_mime_init(curl.get());
    curl_mimepart *part = curl_mime_addpart(multipart);
    curl_mime_name(part, "image");
    curl_mime_filedata(part, filename.c_str());
    curl_easy_setopt(curl.get(), CURLOPT_MIMEPOST, multipart);

    CURLcode res = curl_easy_perform(curl.get());
    if(res != CURLE_OK)
    {
        std::cout << "Something happened in http_post\n";
        curl_mime_free(multipart);
        throw Request_exception();
    }

    curl_mime_free(multipart);

    return callback;
}

std::string Request::http_get(const std::string &url)
{
    // debug
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    // url
    curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());

    // user agent
    curl_easy_setopt(curl.get(), CURLOPT_USERAGENT, "curl 7.64.1 (x86_64-pc-linux-gnu) libcurl/7.64.1");

    // etc
    curl_easy_setopt(curl.get(), CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl.get(), CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl.get(), CURLOPT_ACCEPT_ENCODING, "gzip");

    // callback
    std::string callback;
    curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA,     &callback);

    CURLcode res =  curl_easy_perform(curl.get());

    if(res != CURLE_OK)
    {
        std::cout << "Something happened in http_post\n";
        throw Request_exception();
    }

    return callback;
}

} // namespace request