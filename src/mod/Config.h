#pragma once

#include "ll/api/Config.h"
#include <string>
#include <vector>

namespace my_mod {

struct Config {
    int64 version = 6;

    bool whitelistMode = true; 
    std::vector<std::string> targetFamilies = {"zombie"};

    float densityMultiplier = 2.0f;

    float globalCapMultiplier = 1.0f;
}; 

} // namespace my_mod