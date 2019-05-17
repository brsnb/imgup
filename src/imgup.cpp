#include "imgup.hpp"

#include "request.hpp"

namespace imgup
{

std::string upload_imgur(const std::string &filename)
{
    std::string url{"https://api.imgur.con/3/image"};
    std::vector<std::string> headers{"Authorization: Client-ID 8d7d119014fab88",
                                     "Expect: "};

    request::Request post{};
    std::string response = post.http_image_post(url, headers, filename);
}

} // namespace imgup
