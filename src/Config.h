#pragma once

#include "ll/api/Config.h"
#include <string>
#include <vector>

namespace SpawnerSetting {

struct Config {
    int64 version = 9;

    bool whitelistMode = true; 

    bool enableFamilyFilter = true; 
    std::vector<std::string> targetFamilies = {"zombie"};

    bool enableIdentifierFilter = true; 
    std::vector<std::string> targetMonsterIds = {}; 

    float densityMultiplier = 2.0f;
    float globalCapMultiplier = 1.0f;

    // 设置为 2 表示 2 倍速 即每刻尝试生成 2 次 
    int spawnSpeed = 10; 
}; 

} // namespace SpawnerSetting