#include "Main.h"

#include "ll/api/memory/Hook.h"
#include "ll/api/memory/Signature.h"
#include "ll/api/memory/Memory.h"

#include "mc/world/actor/Mob.h"
#include "mc/world/level/dimension/Dimension.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/Spawner.h"
#include "mc/deps/core/string/HashedString.h"
#include "mc/world/actor/ActorDefinitionIdentifier.h"

#include <array>
#include <string>

using namespace ll::literals::memory_literals;

namespace {

using MobDensityArray = std::array<float, 7>;

void applyDensityMultiplier(Dimension* dim) {
    auto& config = SpawnerSetting::Spawner::getInstance().getConfig();
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
    const auto& config = SpawnerSetting::Spawner::getInstance().getConfig();

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

LL_AUTO_STATIC_HOOK(
    SpawnerTickHook,
    ll::memory::HookPriority::Normal,
    "48 8B C4 4C 89 48 ? 55 53"_sig,
    uint64_t,
    ::Spawner* spawner,
    int64_t* a2,
    int64_t a3,
    void* a4
) {
    auto& config = SpawnerSetting::Spawner::getInstance().getConfig();
    float multiplier = config.globalCapMultiplier;

    if (multiplier <= 0.0f || multiplier == 1.0f) {
        return origin(spawner, a2, a3, a4);
    }

    auto& mobCount = ll::memory::dAccess<unsigned int>(spawner, 552);
    unsigned int realCount = mobCount;

    unsigned int fakeCount = static_cast<unsigned int>(realCount / multiplier);
    mobCount = fakeCount;

    auto result = origin(spawner, a2, a3, a4);

    unsigned int newFakeCount = mobCount;
    int diff = (int)newFakeCount - (int)fakeCount;
    mobCount = realCount + diff;

    return result;
}

} // namespace