#include "mod/Main.h"

#include "ll/api/mod/RegisterHelper.h"
#include "ll/api/io/Logger.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/Mob.h"
#include "mc/world/actor/Actor.h"
#include "mc/deps/core/string/HashedString.h"

namespace {

LL_AUTO_TYPE_INSTANCE_HOOK(
    CheckSpawnRulesHook,
    ll::memory::HookPriority::Normal,
    Mob,
    
    &Mob::$checkSpawnRules,
    
    bool,
    bool checkSpawnPosition
) {
    if (this->hasFamily(HashedString("zombie"))) {
        return origin(checkSpawnPosition); //
    }

    return false;
}

}

namespace my_mod {

MyMod& MyMod::getInstance() { //
    static MyMod instance;
    return instance;
}

bool MyMod::load() { //
    getSelf().getLogger().info("加载 MobSpawnSetting 中...");
    getSelf().getLogger().info("Author: @2PuceWriter79890 - Github RE-Studio-CN");
    return true;
}

bool MyMod::enable() { //
    getSelf().getLogger().info("启用 MobSpawnSetting 中...");
    getSelf().getLogger().info("Hook 成功");
    return true;
}

bool MyMod::disable() { //
    getSelf().getLogger().info("禁用 MobSpawnSetiing 中...");
    getSelf().getLogger().info("Hook 已卸载");
    return true;
}

} // namespace my_mod

LL_REGISTER_MOD(my_mod::MyMod, my_mod::MyMod::getInstance());