#ifndef ATTACK_HPP
#define ATTACK_HPP

#include "Util/GameObject.hpp"
#include "Effect/CompositeEffect.hpp"
#include "Character.hpp"
#include "Util/Text.hpp"
#include "Util/Time.hpp"
#include "Util/Color.hpp"

// 攻擊類型枚舉
enum class AttackType {
    INSTANT,    // 無延遲立即攻擊
    DELAYED,    // 有delay timer
    SEQUENCED   // 有delay timer + 編號
};

// 基礎攻擊類
class Attack : public Util::GameObject {
public:
    // 攻擊狀態枚舉
    enum class State {
        WARNING,    // 顯示警告
        COUNTDOWN,  // 倒數計時
        ATTACKING,  // 攻擊中
        FINISHED    // 攻擊結束
    };
    
    // 構造函數
    Attack(const glm::vec2& position, float delay, float attackDuration, float damage);
    virtual ~Attack() = default;
    
    // 更新攻擊狀態
    virtual void Update(float deltaTime);
    
    // 檢查是否命中角色
    bool CheckHit(std::shared_ptr<Character> character);
    
    // 取得當前狀態
    State GetState() const { return m_State; }
    
    // 取得攻擊位置
    const glm::vec2& GetPosition() const { return m_Position; }
    
    // 取得攻擊傷害值
    float GetDamage() const { return m_Damage; }
    
    // 判斷攻擊是否結束
    bool IsFinished() const { return m_State == State::FINISHED; }

protected:
    State m_State = State::WARNING;     // 當前攻擊狀態
    glm::vec2 m_Position;               // 攻擊位置
    float m_Delay;                      // 警告延遲時間
    float m_AttackDuration;             // 攻擊持續時間
    float m_Damage;                     // 攻擊傷害值
    float m_Timer = 0.0f;               // 計時器
    float m_HitboxSize = 100.0f;        // 碰撞箱大小
    
    std::shared_ptr<Effect::CompositeEffect> m_WarningEffect;   // 警告特效
    std::shared_ptr<Effect::CompositeEffect> m_ProgressBar;     // 進度條特效
    std::shared_ptr<Effect::CompositeEffect> m_AttackEffect;    // 攻擊特效
    
    // 子類必須實現的方法
    virtual void CreateWarningEffect() = 0;    // 創建警告特效
    virtual void CreateProgressBar() = 0;      // 創建進度條特效
    virtual void CreateAttackEffect() = 0;     // 創建攻擊特效
    virtual void UpdateProgressBar();          // 更新進度條狀態
    
    // 碰撞檢測
    virtual bool CheckCollision(std::shared_ptr<Character> character);
    
    // 切換到攻擊階段
    virtual void SwitchToAttack();
    
    // 結束攻擊
    virtual void FinishAttack();
};

#endif // ATTACK_HPP