#include "SpawnerMod.h"

#include "ll/api/memory/Hook.h"
#include "ll/api/memory/Signature.h"
#include "ll/api/memory/Memory.h"
#include "ll/api/io/Logger.h"

#include "mc/world/actor/Mob.h"
#include "mc/world/level/dimension/Dimension.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/Spawner.h"
#include "mc/deps/core/string/HashedString.h"
#include "mc/world/actor/ActorDefinitionIdentifier.h"

#include <string>

using namespace ll::literals::memory_literals;

namespace SpawnerSetting {

namespace {

void applyDensityMultiplier(Dimension* dim) {
    auto& config = SpawnerMod::getInstance().getConfig();
    float multiplier = config.densityMultiplier;
    auto& logger = SpawnerMod::getInstance().getSelf().getLogger();

    if (multiplier == 1.0f) return;

    float originalVal = dim->mMobsPerChunkSurface[0];

    for (float& val : dim->mMobsPerChunkSurface) {
        val *= multiplier;
    }
    for (float& val : dim->mMobsPerChunkUnderground) {
        val *= multiplier;
    }

    logger.info("维度 ID: {} | 密度倍率: {:.1f} | 地表密度上限: {:.1f} -> {:.1f}",
        (int)dim->getDimensionId(), multiplier, originalVal, dim->mMobsPerChunkSurface[0]);
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
    const auto& config = SpawnerMod::getInstance().getConfig();

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
    "48 8B C4 4C 89 48 20 55 53 56 57 41 54 41 55 41 56 41 57 48 8D"_sig,
    // 1.21.130.4: 48 8B C4 4C 89 48 ? 55
    void,
    ::Spawner* spawner,
    ::BlockSource& region,
    ::LevelChunkVolumeData const& volumeData,
    ::ChunkPos const chunkPos
) {
    auto& config = SpawnerMod::getInstance().getConfig();
    float multiplier = config.globalCapMultiplier;
    int speed = config.spawnSpeed;
    if (speed < 1) speed = 1;

    auto& mobCount = ll::memory::dAccess<unsigned int>(spawner, 552);
    unsigned int currentRealCount = mobCount;

    for (int i = 0; i < speed; ++i) {
        if (multiplier <= 0.0f || multiplier == 1.0f) {
            origin(spawner, region, volumeData, chunkPos);
            continue;
        }

        unsigned int fakeCount = static_cast<unsigned int>(currentRealCount / multiplier);

        if (fakeCount >= 200) {
            if (i == 0) {
                mobCount = currentRealCount;
                origin(spawner, region, volumeData, chunkPos);
                currentRealCount = mobCount;
            }
            break;
        }

        mobCount = fakeCount;
        origin(spawner, region, volumeData, chunkPos);

        unsigned int newMemCount = mobCount;
        int delta = (int)newMemCount - (int)fakeCount;
        currentRealCount += delta;
        
        mobCount = currentRealCount;
    }
}

}
} // namespace SpawnerSetting