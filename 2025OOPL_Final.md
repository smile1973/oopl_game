# 2025 OOPL Final Report

## 組別資訊

組別： 31
組員： 
- 111820024 廖翊宏
- 111820030 楊承諭

復刻遊戲：Rabbit and Steel

## 專案簡介

### 遊戲簡介

### 組別分工
111820024 廖翊宏：
- 遊戲主架構 (App.cpp, main.cpp)
- 角色系統 (Character.cpp)
- 敵人系統 (Enemy.cpp) 
- UI系統 (LevelUI, HealthBarUI, ShopUI, DefeatScreen)
- 遊戲階段管理 (PhaseManager)
- 物件碰撞系統 (Collides.cpp)

111820030 楊承諭：
- 完整特效系統 (Effect目錄下所有內容)
- 完整攻擊系統 (Attack目錄下所有內容)
- 攻擊模式工廠 (AttackPatternFactory)
- 部分角色功能
- 技能實作

## 遊戲介紹

### 遊戲規則
### 遊戲畫面

## 程式設計

### 程式架構
本專案採用物件導向設計，主要架構包含：

1. **主程式架構**
   - App類別：使用單例模式管理整個遊戲的狀態機(START, UPDATE, END)
   - PhaseManager：管理遊戲中的不同階段轉換

2. **遊戲物件系統**
   - Object基礎類別：所有遊戲物件的基底類別
   - Character：玩家角色類別，管理移動、技能、血量等
   - Enemy：敵人類別，包含不同的攻擊模式和AI行為
   - Skill：技能系統，支援動畫播放和冷卻時間管理

3. **攻擊與特效系統**
   - AttackManager：管理所有攻擊物件的生命週期
   - AttackPattern：攻擊模式的基礎類別
   - AttackPatternFactory：使用工廠模式產生各種攻擊模式
   - EffectManager：管理遊戲中所有視覺特效
   - CompositeEffect：組合模式實現複雜特效

4. **UI系統**
   - HealthBarUI：玩家血條顯示
   - SkillUI：技能冷卻時間顯示
   - LevelUI：關卡資訊顯示
   - ShopUI：商店介面
   - DefeatScreen：失敗畫面

### 程式技術
1. **設計模式應用**
   - 單例模式(Singleton)：App、EffectManager、AttackManager
   - 工廠模式(Factory)：AttackPatternFactory、EffectFactory
   - 組合模式(Composite)：CompositeEffect用於組合多個特效

2. **遊戲開發技術**
   - 物件池(Object Pooling)：用於優化大量彈幕的生成與回收
   - 狀態機(State Machine)：管理角色和技能的狀態轉換
   - 碰撞檢測：實現精確的彈幕判定系統

## 結語

### 問題與解決方法
1. **彈幕效能優化問題**
   - 問題：大量彈幕同時存在時造成FPS下降
   - 解決：實作物件池機制，重複使用攻擊物件，減少記憶體配置

2. **攻擊模式同步問題**
   - 問題：多個敵人的攻擊時機難以協調
   - 解決：使用AttackController統一管理攻擊時序

3. **特效層級顯示問題**
   - 問題：特效與遊戲物件的渲染順序混亂
   - 解決：實作層級管理系統，確保正確的繪製順序

### 自評

| 項次 | 項目                   | 完成 |
|------|------------------------|-------|
| 1    | 基本角色移動與技能系統 |  V  |
| 2    | 敵人AI與攻擊模式      |  V  |
| 3    | 彈幕系統與碰撞判定    |  V  |
| 4    | 視覺特效系統          |  V  |
| 5    | UI介面與血條顯示      |  V  |
| 6    | 音效與背景音樂        |     |

### 心得
透過這次專案，我們深入學習了遊戲開發的各個面向，從基礎的物件導向設計到進階的效能優化技術。特別是在實作彈幕系統時，體會到了設計模式的重要性，以及如何在效能與功能之間取得平衡。團隊合作方面，我們學會了如何有效分工，並透過良好的程式架構讓雙方的程式碼能夠順利整合。

### 貢獻比例
- 111820024 廖翊宏：50%
- 111820030 楊承諭：50%