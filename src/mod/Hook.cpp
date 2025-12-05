#include "ll/api/memory/Hook.h"
#include "mc/world/actor/Mob.h"
#include "mc/world/actor/Actor.h"
#include "mc/deps/core/string/HashedString.h"
#include "mod/Main.h"

namespace {

LL_AUTO_TYPE_INSTANCE_HOOK(
    CheckSpawnRulesHook,
    ll::memory::HookPriority::Normal,
    Mob,
    &Mob::$checkSpawnRules,
    bool,
    bool checkSpawnPosition
) {
    const auto& config = my_mod::MyMod::getInstance().getConfig();
    
    bool inList = false;
    for (const auto& familyName : config.targetFamilies) {
        if (this->hasFamily(HashedString(familyName.c_str()))) {
            inList = true;
            break;
        }
    }

    if (config.whitelistMode) {
        if (inList) {
            return origin(checkSpawnPosition);
        } else {
            return false;
        }
    } else {
        if (inList) {
            return false;
        } else {
            return origin(checkSpawnPosition);
        }
    }
}

} // namespace