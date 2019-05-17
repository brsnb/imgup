#pragma once

#include <string>

namespace imgup
{

/*
*   Uploads file at filename to imgur and returns link
*/
std::string upload_imgur(const std::string &filename);

} // namespace imgup
