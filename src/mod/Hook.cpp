#include "ll/api/memory/Hook.h"
#include "mc/world/actor/Mob.h"
#include "mc/world/level/dimension/Dimension.h"
#include "mc/world/level/Level.h"   
#include "mc/world/level/Spawner.h" 
#include "mc/deps/core/string/HashedString.h"
#include "mc/world/actor/ActorDefinitionIdentifier.h" 
#include "mod/Main.h"
#include <array>
#include <string>

namespace {

using MobDensityArray = std::array<float, 7>;

void applyDensityMultiplier(Dimension* dim) {
    auto& config = my_mod::MyMod::getInstance().getConfig();
    float multiplier = config.densityMultiplier;

    if (multiplier == 1.0f) return;

    auto* surfaceArray = reinterpret_cast<MobDensityArray*>(&dim->mMobsPerChunkSurface);
    auto* undergroundArray = reinterpret_cast<MobDensityArray*>(&dim->mMobsPerChunkUnderground);

    for (float& val : *surfaceArray) val *= multiplier;
    for (float& val : *undergroundArray) val *= multiplier;
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    DimensionInitHook,
    ll::memory::HookPriority::Normal,
    Dimension,
    &Dimension::$init, 
    void,
    ::br::worldgen::StructureSetRegistry const& structureSetRegistry
) {
    origin(structureSetRegistry);
    applyDensityMultiplier(this);
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    CheckSpawnRulesHook,
    ll::memory::HookPriority::Normal,
    Mob,
    &Mob::$checkSpawnRules,
    bool,
    bool checkSpawnPosition
) {
    const auto& config = my_mod::MyMod::getInstance().getConfig();

    bool isFamilyMatch = false;
    bool isIdMatch = false;

    if (config.enableFamilyFilter) {
        for (const auto& familyName : config.targetFamilies) {
            if (this->hasFamily(HashedString(familyName.c_str()))) {
                isFamilyMatch = true;
                break; 
            }
        }
    }

    if (config.enableIdentifierFilter) {
        std::string myId = this->getActorIdentifier().getFullName(); 
        for (const auto& targetId : config.targetMonsterIds) {
            if (myId == targetId) {
                isIdMatch = true;
                break;
            }
        }
    }

    bool isTarget = isFamilyMatch || isIdMatch;

    if (config.whitelistMode) {
        if (!isTarget) return false;
    } else {
        if (isTarget) return false;
    }

    return origin(checkSpawnPosition);
}

static bool isSpawnerHooked = false;
using SpawnerCountFunc = unsigned int (*)(void*);
static SpawnerCountFunc originalSpawnerCount = nullptr;

unsigned int DetourGetMobCount(void* self) {
    if (!originalSpawnerCount) return 0;
    unsigned int realCount = originalSpawnerCount(self);

    const auto& config = my_mod::MyMod::getInstance().getConfig();
    if (config.globalCapMultiplier > 1.0f) {
        return static_cast<unsigned int>(realCount / config.globalCapMultiplier);
    }
    return realCount;
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    LevelTickHook,
    ll::memory::HookPriority::Normal,
    Level,
    &Level::$tick, 
    void
) {
    origin();

    if (!isSpawnerHooked) {
        auto& spawner = this->getSpawner();
        void** vtable = *reinterpret_cast<void***>(&spawner);
        void* targetFunctionAddress = vtable[15];

        ll::memory::hook(
            targetFunctionAddress,
            reinterpret_cast<void*>(&DetourGetMobCount),
            reinterpret_cast<void**>(&originalSpawnerCount),
            ll::memory::HookPriority::Normal
        );

        isSpawnerHooked = true;
        my_mod::MyMod::getInstance().getSelf().getLogger().info("Global MobCap Hook installed successfully!");
    }
}

} // namespace