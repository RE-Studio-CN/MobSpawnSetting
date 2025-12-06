**MobSpawnSettings** 是专为生存模式开发的插件。其提供了生物生成控制功能，允许服主自定义生物生成的黑白名单、突破原版区块密度限制，以及游戏的全局生物上限。

## 主要功能

1. **黑/白名单控制
    
    - **白名单模式**：只允许配置列表中的生物家族（如 `zombie` 族）生成，其他所有自然生成的生物都会被禁止。
    
    - **黑名单模式**：禁止配置列表中的生物生成，其他生物正常生成。适合移除某些烦人的怪物（如苦力怕）。
    
2. **区块密度倍增
    
    - 设置为 `2.0` 或更多表示每个区块允许容纳的怪物数量翻倍。
    
    - 设置为 `0.5` 则减半。解决刷怪太多的问题。
    
3. **修改全局生物生成上限
    
    - 在基岩版中，无论难度如何，自然生成的全局上限总是200。更改配置文件即可降低或提高上限。
    
4. **生物 ID / 族控制
    
    - 可选择控制生物族或控制单个生物的生成
    
---

## 配置文件 (config.json)

首次加载后会在 `plugins/MobSpawnSettings/config.json` 生成配置文件。

### 默认配置

``` json
{
    "version": 8,
    "whitelistMode": true,  // 工作模式: true=白名单, false=黑名单
    "enableFamilyFilter": true,  // 是否启用族过滤
    "targetFamilies": [
        "zombie"
    ],  // 目标族，可参考 https://minecraft.fandom.com/zh/wiki/%E6%97%8F
    "enableIdentifierFilter": true,  // 是否启用 ID 过滤
    "targetMonsterIds": [
        "minecraft:creeper"
    ],  // 目标 ID (例如 "minecraft:creeper")
    "densityMultiplier": 2.0,  // 局部密度倍率 (大于 1.0 增加密度)
    "globalCapMultiplier": 1.0  // 全局上限倍率 (大于 1.0 为提高，小于则减少)
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
    "globalCapMultiplier": 1.0
}
```

### 刷怪塔优化 (高密度)

允许所有怪生成，提高生成上限。

``` json
{
    "version": 8,
    "whitelistMode": false,
    "enableFamilyFilter": true,
    "targetFamilies": [],
    "enableIdentifierFilter": false,
    "targetMonsterIds": [],
    "densityMultiplier": 4.0,
    "globalCapMultiplier": 4.0
}
```

_(注：黑名单模式下列表为空 = 允许所有生物)_

---

## ⚠️ 性能警告

- **关于 `globalCapMultiplier`**：将全局上限设置为 `6.0` 或更高时，服务器内可能会瞬间存在数百甚至上千个实体。使用建议将倍率设置在 `1.5` ~ 5.5 之间进行测试。

---

## 安装

下载该插件。放入`plugins` 内，启动服务器，自动生成配置文件。之后修改 `plugins/MobSpawnSettings/config.json` 重启服务器配置生效。