#include "mod/Main.h"

#include "ll/api/Config.h"
#include "ll/api/mod/RegisterHelper.h"
#include "ll/api/io/Logger.h"

namespace my_mod {

MyMod& MyMod::getInstance() {
    static MyMod instance;
    return instance;
}

bool MyMod::load() {
    getSelf().getLogger().info("加载 AllowOnlyFamily 中...");
    getSelf().getLogger().info("Author: Xiao_Puce_awa - Github @2PuceWriter79890");

    std::filesystem::path configPath = getSelf().getModDir() / "config.json";
    if (!ll::config::loadConfig(mConfig, configPath)) {
        getSelf().getLogger().warn("无法读取 Config.json ，自动生成默认配置文件中...");
        if (!ll::config::saveConfig(mConfig, configPath)) {
            getSelf().getLogger().error("无法保存配置文件");
            return false;
        }
    }

    return true;
}

bool MyMod::enable() {
    getSelf().getLogger().info("启用 AllowOnlyFamily 中...");
    return true;
}

bool MyMod::disable() {
    getSelf().getLogger().info("启用 AllowOnlyFamily 中...");
    getSelf().getLogger().info("Hook 已被卸载");
    return true;
}

} // namespace

LL_REGISTER_MOD(my_mod::MyMod, my_mod::MyMod::getInstance());