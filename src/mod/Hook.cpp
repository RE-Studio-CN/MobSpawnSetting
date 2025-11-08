#include "ll/api/memory/Hook.h"
#include "mc/world/actor/Mob.h"
#include "mc/world/actor/Actor.h"
#include "mc/deps/core/string/HashedString.h"
#include "mod/Main.h"
#include "mod/Config.h"

namespace {

LL_AUTO_TYPE_INSTANCE_HOOK(
    CheckSpawnRulesHook,
    ll::memory::HookPriority::Normal,
    Mob,
    &Mob::$checkSpawnRules,
    bool,
    bool checkSpawnPosition
) {
    const auto& allowedFamilies = my_mod::MyMod::getInstance().getConfig().allowedFamilies;

    for (const auto& familyName : allowedFamilies) {
        if (this->hasFamily(HashedString(familyName.c_str()))) {
            return origin(checkSpawnPosition);
        }
    }
    
    return false;
}

} // namespace 