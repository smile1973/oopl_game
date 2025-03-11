# ?
- 素材讀取方式建立 (這個框架設計好像無法?)
  - 一張png多個動作，切分出的坐標系儲存
  - 該怎麼存最有效率? 如何分類?

- 邊界
  - [ ] 目前可以移出地圖外
- 兔子的技能設計
  - [ ] 攻擊的效果顯示
- 攻擊動畫
  - [x] 嘗試看看可不可以在主技能上加上動畫(先以這個做為技能有觸發的判斷)
  - [x] 加上剩下的技能

- (bouns) bullet class 設計
- 技能UI
- 視窗大小怎麼調整

- 敵人
  - [ ] hitbox 判定
  - [ ] 先做出一個假人 賦予該有的屬性
    - 應該有什麼屬性? 生命 圖片 ...
  - [ ] 做出假人後 和技能做碰撞辦定看看
- 怎麼切?
  - 動畫是不是要開另一個class??
  - 技能 移動 閒置 都有動畫 這樣應該不能綁在character內?
  - 還是每一個類都放? -> 這個好像是優解

### Rabbit 素材
- hb_rabbit_idle1 ~ 2 : 閒置動畫
- hb_rabbit_forward1 ~ 2 : up down forward
- hb_rabbit_backward1 ~ 2 : backward
- hb_rabbit_gethit : get_hit
- 7 - lie_down
- hb_rabbit_skill2_1 ~ 5 : skill2(X)
- hb_rabbit_skill1_1 ~ 6 : skill1 & skill3(Z C)
- hb_rabbit_skill3_1 ~ 4 : skill4(V)

底線(_)開頭代表沒用到
***
1. 敵人類別 Enemy class
   - 該有的屬性要想一下 
2. 利用class 做一個假人(dummy) 要把圖片放上去
3. 受擊判定
   - 先做一個技能的就好 試試看
   - 可以利用 log_debug 看看有沒有讀到?
4. (bonus) 敵人血條UI

***
渲染流程
1. CPU端準備數據:
   - 計算變換矩陣
   - 設置特效參數(半徑、顏色等)
2. 數據傳輸到GPU:
   - 通過SetData將矩陣數據傳入uniform緩衝區
   - 通過glUniform*函數設置其他著色器參數
3. GPU渲染管線
   - 頂點著色器(Circle.vert):
     - 接收頂點位置和紋理座標
     - 應用矩陣變換計算最終頂點位置
     - 將紋理座標傳遞給片段著色器
   - 片段著色器(Circle.frag):
     - 計算每個片段(像素)的顏色
     - 使用數學函數計算圓形
     - 應用脈動效果和漸變
     - 輸出最終像素顏色
4. 混合與顯示:
   - 將計算出的片段與幀緩衝區中的像素混合
   - 最終結果顯示在屏幕上
***

App.hpp
  - m_Background (BackgroundImage) : 背景
  - m_Rabbit (Character): 兔子

Skill.hpp
  - m_ImagePathSet (vector<string>) : 技能動畫圖片路徑
  - m_Animation (Util::Animation) : 技能動畫物件
  - m_State : 技能狀態 IDLE / ACTIVE
  - m_duration (int) : 技能動畫持續時間
  - m_EffectRadius (float) : 特效大小
  - m_EffectColor (Util::Color) : 特效顏色
  - m_CurrentEffect (Effect::IEffect) : 只是用在追蹤目前特效 現在沒用 看之後要不要用

Character.hpp
  - m_ImagePathSet (vector<string>) : 腳色閒置動畫圖片路徑
  - m_Animation (Util::Animation) : 腳色閒置動畫物件
  - m_skill (map<skillid(int), Skill>) : 存所有技能
  - m_State : 技能狀態 IDLE / USING_SKILL
  - m_CurrentSkillId (int) : 目前技能ID -1 / 1 ~ 4
  - m_CurrentSkill (Skill) : 目前技能

IEffect.hpp <- Drawable
  - m_State : 特效狀態 INACTIVE / ACTIVE / FINISHED
  - m_Transform : 存儲特效的變換資訊(位置、旋轉、縮放)  
    用於計算特效在世界空間中的位置和大小  
    在Draw()方法中用於創建model矩陣
  - m_Duration : 特效持續時間
  - m_ElapsedTime : 特效已撥放時間
  - m_Duration : 特效繪製順序

CircleEffect.hpp <- Effect.hpp
  - s_Program (Core::Program) : 著色器 所有CircleEffect共享同一個
  - s_VertexArray (Core::VertexArray) : 存特效的幾何形狀數據 四邊形的頂點座標和紋理座標
  - m_MatricesBuffer (Core::UniformBuffer<Core::Matrices>) : 存變換矩陣 用於定位和縮放特效
  - m_RadiusLocation (GLint) : 著色器中 u_Radius uniform變數的位置 用於設置圓的半徑
  - m_ThicknessLocation (GLint) : u_Thickness uniform變數的位置 控制圓的邊緣寬度
  - m_FadeLocation (GLint) : 圓邊緣的漸變效果
  - m_ColorLocation (GLint) : 圓的顏色
  - m_TimeLocation (GLint) : u_Time uniform變數的位置 傳遞時間 實現pulse
  - m_Radius, m_Thickness, m_FadeWidth (float) : 圓半徑 邊緣寬度 漸變效果寬度
  - m_Color (Util::Color) : 圓顏色
  - m_Size (glm::vec2) : 特效基底大小

EffectManager.hpp <- GameObject
  - m_InactiveEffects (map< EffectType, queue<IEffect> >) : 特效物件池
  - m_ActiveEffects (vector<IEffect>) : 當前活躍的特效

EffectFactory.hpp
  - m_Creators (map< EffectType, function<IEffect>() >) : 特效創建函數註冊表

***
# Task
- 用Z之前 X V不能正常顯示(未解決)
- 特效會有覆蓋問題 看是要寫渲染順序(麻煩) 還是 特效改中空(簡單)
- duration如果是傳給shape的話 她船不進去composite effect 造成秒數沒變(待修)
- 有時候雷射會少一根 why? -> 應該是回去時沒reset? -> 角度不正確
