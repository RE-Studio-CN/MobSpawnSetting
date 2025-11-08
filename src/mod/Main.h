#pragma once

#include "ll/api/mod/NativeMod.h"
#include "mod/Config.h"

namespace my_mod {

class MyMod {

public:
    static MyMod& getInstance();

    MyMod() : mSelf(*ll::mod::NativeMod::current()) {}

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    [[nodiscard]] Config const& getConfig() const { return mConfig; }

    bool load();
    bool enable();
    bool disable();

private:
    ll::mod::NativeMod& mSelf;
    Config              mConfig;
};

} // namespace