# TODO
- 素材讀取方式建立 (這個框架設計好像無法?)
  - 一張png多個動作，切分出的坐標系儲存
  - 該怎麼存最有效率? 如何分類?


- 邊界
  - [ ] 目前可以移出地圖外
- 兔子的技能設計
  - [ ] 攻擊的效果顯示
- 攻擊動畫
  - [x] 嘗試看看可不可以在主技能上加上動畫(先以這個做為技能有觸發的判斷)
  - [ ] 加上剩下的技能
- 視窗大小怎麼調整
- 嘗試渲染看看主技能
- bullet class 設計
  
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

1. 敵人類別 Enemy class
   - 該有的屬性要想一下 
2. 利用class 做一個假人(dummy) 要把圖片放上去
3. 受擊判定
   - 先做一個技能的就好 試試看
   - 可以利用 log_debug 看看有沒有讀到?
   - 
4. (bonus) 敵人血條UI