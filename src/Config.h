#pragma once

#include "ll/api/Config.h"
#include <string>
#include <vector>

namespace SpawnerSetting {

struct Config {
    int64 version = 8;

    bool whitelistMode = true; 

    bool enableFamilyFilter = true; 
    std::vector<std::string> targetFamilies = {"zombie"};

    bool enableIdentifierFilter = true; 
    std::vector<std::string> targetMonsterIds = {}; 

    float densityMultiplier = 2.0f;
    float globalCapMultiplier = 1.0f;
}; 

} // namespace SpawnerSetting