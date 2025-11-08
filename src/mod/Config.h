#pragma once

#include "ll/api/Config.h" 

#include <string>
#include <vector>

namespace my_mod {

struct Config {
    int64 version = 1;

    std::vector<std::string> allowedFamilies = {"zombie"};
}; 

} // namespace