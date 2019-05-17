#include "imgup.hpp"

#include <exception>
#include <nlohmann/json.hpp>
#include "request.hpp"

namespace imgup
{

std::string upload_imgur(const std::string &filename)
{
    // set up request
    std::string url{"https://api.imgur.com/3/image"};
    std::vector<std::string> headers{"Authorization: Client-ID 8d7d119014fab88",
                                     "Expect: "};

    // send image
    request::Request post{};
    std::string response = post.http_image_post(url, headers, filename);

    // parse response
    using json = nlohmann::json;
    json res_json = json::parse(response);

    if(res_json["success"].get<bool>() != true)
    {
        throw std::runtime_error("Something happened, probably a missing file");
    }

    return res_json["data"]["link"].get<std::string>();
}

} // namespace imgup
