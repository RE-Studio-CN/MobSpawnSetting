**MobSpawnSettings** 是专为生存模式开发的插件。其提供了生物生成控制功能，允许管理员自定义生物生成的黑白名单、突破原版区块密度限制等。

---

## 主要功能

1. **黑/白名单控制**
    
    - 白名单模式：只允许配置列表中的生物家族（如 `zombie` 族）生成，其他所有自然生成的生物都会被禁止。
    
    - 黑名单模式：禁止配置列表中的生物生成，其他生物正常生成。适合移除某些烦人的怪物（如苦力怕）。
    
2. **区块密度倍增**
    
    - 设置为 `2.0` 或更多表示每个区块允许容纳的怪物数量翻倍。
    
    - 设置为 `0.5` 则减半。解决刷怪太多的问题。
    
3. **修改全局生物生成上限**
    
    - 在基岩版中，无论难度如何，自然生成的全局上限总是200。更改配置文件即可降低或提高上限。
    
4. **生物 ID / 族控制**
    
    - 可选择控制生物族或控制单个生物的生成

5. **修改生物生成速度**

    - 加快生物生成速度
    
---

## 配置文件 (config.json)

首次加载后会在 `plugins/MobSpawnSettings/config.json` 生成配置文件。

### 默认配置
``` json5
{
    "version": 8,
    "whitelistMode": false,             // 工作模式: true=白名单 false=黑名单
    "enableFamilyFilter": true,         // 是否启用族过滤
    "targetFamilies": [
        "zombie"                        // 目标族 可参考 https://minecraft.fandom.com/zh/wiki/%E6%97%8F
    ],
    "enableIdentifierFilter": false,
    "targetMonsterIds": [
        "minecraft:creeper"             // 目标 ID
    ],
    "densityMultiplier": 4.0,           // 局部密度倍率 (大于 1.0 增加密度，小于则减少)
    "globalCapMultiplier": 4.0,         // 全局上限倍率 (大于 1.0 为提高，小于则减少)
    "spawnSpeed": 2                     // 尝试生成速度 设置为 2 表示每刻尝试生成 2 次
}
```

## 配置文件示例

### 禁止苦力怕生成

不想要苦力怕生成去炸家。

``` json
{
    "version": 8,
    "whitelistMode": false,
    "enableFamilyFilter": true,
    "targetFamilies": [
        "creeper"
    ],
    "enableIdentifierFilter": false,
    "targetMonsterIds": [],
    "densityMultiplier": 1.0,
    "globalCapMultiplier": 1.0,
    "spawnSpeed": 1
}
```

### 刷怪塔优化 (高密度)

允许所有怪生成，提高生成上限。

``` json
{
    "version": 8,
    "whitelistMode": false,
    "enableFamilyFilter": false,
    "targetFamilies": [],
    "enableIdentifierFilter": false,
    "targetMonsterIds": [],
    "densityMultiplier": 4.0,
    "globalCapMultiplier": 4.0,
    "spawnSpeed": 100
}
```

### 整活（别在正式服里玩）

加快生成速度和生成上限。

``` json
{
    "version": 8,
    "whitelistMode": false,
    "enableFamilyFilter": false,
    "targetFamilies": [],
    "enableIdentifierFilter": false,
    "targetMonsterIds": [],
    "densityMultiplier": 2048.0,
    "globalCapMultiplier": 32.0,
    "spawnSpeed": 1000
}
```

_注：黑名单模式下列表为空 = 允许所有生物_

---

## ⚠️ 性能警告

- **将 `globalCapMultiplier` 和 `spawnSpeed` 同时调高时会让世界上瞬间生成几百个实体**

---

## 安装（服务端）

### 使用 LIP

`lip install github.com/RE-Studio-CN/MobSpawnSetting`

### 手动安装

从 **Releases** 下载该插件，解压到 `plugins` 内。

---

## MineBBS

[MobSpawnSettings](https://www.minebbs.com/resources/mobspawnsettings.14280/)