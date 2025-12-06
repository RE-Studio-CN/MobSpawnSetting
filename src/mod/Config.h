#pragma once

#include "ll/api/Config.h"
#include <string>
#include <vector>

namespace my_mod {

struct Config {
    int64 version = 2;

    bool whitelistMode = true; 

    std::vector<std::string> targetFamilies = {"zombie"};
}; 

} // namespace my_mod