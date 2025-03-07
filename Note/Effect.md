# 特效系統


#### 核心特效類
- `Effect/IEffect.hpp` - 特效base
- `Effect/Shape/BaseShape.hpp` 和 `.cpp` - 形狀 base class
- `Effect/Shape/CircleShape.hpp` 和 `.cpp` - 圓形
- `Effect/Shape/EllipseShape.hpp` 和 `.cpp` - 橢圓形

#### 修飾器類
- `Effect/Modifier/FillModifier.hpp` 和 `.cpp` - 填充
- `Effect/Modifier/EdgeModifier.hpp` 和 `.cpp` - 邊緣
- `Effect/Modifier/MovementModifier.hpp` 和 `.cpp` - 移動
- `Effect/Modifier/AnimationModifier.hpp` 和 `.cpp` - 額外動畫

#### 組合特效
- `Effect/CompositeEffect.hpp` 和 `.cpp` - 複合特效

#### 工廠和管理器
- `Effect/EffectFactory.hpp` 和 `.cpp` - 特效 factory
- `Effect/EffectManager.hpp` 和 `.cpp` - 特效管理器

#### 著色器
- `shaders/Circle.vert` 和 `.frag` - 圓形著色器
- `shaders/Ellipse.vert` 和 `.frag` - 橢圓形著色器

### 構建和測試

編譯並運行遊戲，使用以下功能測試特效：

- 按 Z X C V 使用不同技能和特效
- 按空格鍵在光標位置顯示測試特效

### 設計

- 組合模式實現特效
- 形狀效果 分離
- 用對象池減少內存分配 (效能)
- 共享著色器 (效能)

### 主要組件

1. **base shape effet**
    - 圓形特效：
    - 橢圓形特效：目前只有C

2. **modifier**
    - fill：實心(FILL) 空心(HOLLOW)
    - edge：邊緣加深(DARK) 發光(GLOW)
    - movement：特效移動(移動子彈)
    - animation：其他動畫(未完成)

3. **特效類型**
    - 角色技能特效（Z、X、C、V）
    - 敵人攻擊特效（三種up）

## 著色器說明

特效系統使用兩套著色器：

1. **圓形特效著色器**
    - 使用漸變(smoothstep)創建柔和邊緣
    - 支持各種修飾器參數
    - 使用時間變量實現動畫效果

2. **橢圓形特效著色器**
    - 基於圓形著色器修改，使用不同的距離計算
    - 支持x和y軸不同半徑


## 性能優化
1. 物件池
2. 著色器共享


### 性能: 批處理繪製(batch)