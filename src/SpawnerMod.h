#pragma once

#include "ll/api/mod/NativeMod.h"
#include "Config.h"

namespace SpawnerSetting {

class SpawnerMod {

public:
    static SpawnerMod& getInstance();

    SpawnerMod() : mSelf(*ll::mod::NativeMod::current()) {}

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    [[nodiscard]] Config const& getConfig() const { return mConfig; }

    bool load();
    bool enable();
    bool disable();

private:
    ll::mod::NativeMod& mSelf;
    Config              mConfig;
};

} // namespace SpawnerSetting