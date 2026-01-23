#include "SpawnerMod.h"

#include "ll/api/Config.h"
#include "ll/api/mod/RegisterHelper.h"
#include "ll/api/io/Logger.h"

#include <filesystem>

namespace SpawnerSetting {

SpawnerMod& SpawnerMod::getInstance() {
    static SpawnerMod instance;
    return instance;
}

bool SpawnerMod::load() {
    getSelf().getLogger().info("加载 MobSpawnSetting 中...");
    getSelf().getLogger().info("Author: PuceLi");

    std::filesystem::path configPath = getSelf().getModDir() / "config.json";
    if (!ll::config::loadConfig(mConfig, configPath)) {
        getSelf().getLogger().warn("无法读取 config.json，将生成默认配置文件...");
        if (!ll::config::saveConfig(mConfig, configPath)) {
            getSelf().getLogger().error("无法保存配置文件");
            return false;
        }
    }

    return true;
}

bool SpawnerMod::enable() {
    getSelf().getLogger().info("MobSpawnSetting 已启用");
    return true;
}

bool SpawnerMod::disable() {
    getSelf().getLogger().info("MobSpawnSetting 已禁用");
    return true;
}

} // namespace SpawnerSetting

LL_REGISTER_MOD(SpawnerSetting::SpawnerMod, SpawnerSetting::SpawnerMod::getInstance());