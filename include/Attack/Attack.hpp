#ifndef ATTACK_HPP
#define ATTACK_HPP

#include "Util/GameObject.hpp"
#include "Util/Color.hpp"
#include "Util/Text.hpp"
#include "Effect/CompositeEffect.hpp"
#include "Character.hpp"
#include <memory>

class Attack : public Util::GameObject {
public:
    enum class State {
        CREATED,
        WARNING,
        COUNTDOWN,
        ATTACKING,
        FINISHED
    };

    // Z-index 層次常數定義
    static constexpr float ZINDEX_WARNING_OFFSET = -2.0f;      // 警告特效偏移
    static constexpr float ZINDEX_ATTACK_OFFSET = 0.0f;        // 攻擊特效偏移
    static constexpr float ZINDEX_TIMEBAR_OFFSET = 1.0f;       // 時間條偏移
    static constexpr float ZINDEX_INDICATOR_OFFSET = 2.0f;     // 方向指示器偏移

    // 建構函數
    Attack(const glm::vec2& position, float delay, int sequenceNumber = 0);
    virtual ~Attack() = default;

    // 核心方法
    void Update(float deltaTime);
    void Draw() override;

    // 狀態檢查
    [[nodiscard]] bool IsFinished() const { return m_State == State::FINISHED; }
    State GetState() const { return m_State; }

    // 碰撞檢測
    bool CheckCollision(const std::shared_ptr<Character>& character);

    // 設置攻擊參數
    void SetPosition(const glm::vec2& position);
    void SetDelay(float delay) { m_Delay = delay; }
    void SetSequenceNumber(int number);
    void SetTargetCharacter(std::shared_ptr<Character> target) { m_TargetCharacter = target; }

    // Z-index 管理方法
    void SetAttackZIndex(float zIndex) {
        SetZIndex(zIndex);
        UpdateAllEffectZIndexes();
    }

    float GetWarningZIndex() const { return GetZIndex() + ZINDEX_WARNING_OFFSET; }
    float GetAttackZIndex() const { return GetZIndex() + ZINDEX_ATTACK_OFFSET; }
    float GetTimeBarZIndex() const { return GetZIndex() + ZINDEX_TIMEBAR_OFFSET; }
    float GetIndicatorZIndex() const { return GetZIndex() + ZINDEX_INDICATOR_OFFSET; }

    // Getter 方法
    int GetSequenceNumber() const { return m_SequenceNumber; }
    float GetDelay() const { return m_Delay; }
    float GetElapsedTime() const { return m_ElapsedTime; }
    const glm::vec2& GetAttackPosition() const { return m_Position; }

    void SetAttackDuration(float duration) { m_AttackDuration = duration; }
    float GetAttackDuration() const { return m_AttackDuration; }

    // 獲取特效
    [[nodiscard]] std::shared_ptr<Effect::CompositeEffect> GetWarningEffect() const { return m_WarningEffect; }
    [[nodiscard]] std::shared_ptr<Effect::CompositeEffect> GetAttackEffect() const { return m_AttackEffect; }
    virtual void CleanupVisuals() {};

protected:
    // 各個階段處理的虛函數
    virtual void OnWarningStart();
    virtual void OnWarningUpdate(float deltaTime);
    virtual void OnCountdownStart();
    virtual void OnCountdownUpdate(float deltaTime);
    virtual void OnAttackStart();
    virtual void OnAttackUpdate(float deltaTime);
    virtual void OnFinishedStart();

    // 特效創建的虛函數 - 必須由子類別實現
    virtual void CreateWarningEffect() = 0;
    virtual void CreateAttackEffect() = 0;
    virtual void CreateTimeBar();
    virtual void UpdateTimeBar(float progress);

    // 碰撞處理 - 必須由子類別實現
    virtual bool CheckCollisionInternal(const std::shared_ptr<Character>& character) = 0;
    virtual void SyncWithEffect() {}

    // Z-index 更新方法
    virtual void UpdateAllEffectZIndexes();

    // 基本屬性
    State m_State = State::CREATED;
    bool m_IsFirstUpdate = true;
    glm::vec2 m_Position;
    float m_Delay;
    float m_ElapsedTime = 0.0f;
    int m_SequenceNumber;
    float m_AttackDuration = 0.5f;

    std::shared_ptr<Character> m_TargetCharacter = nullptr;

    // 視覺元素
    std::shared_ptr<Effect::CompositeEffect> m_WarningEffect;
    std::shared_ptr<Effect::CompositeEffect> m_AttackEffect;
    std::shared_ptr<Effect::CompositeEffect> m_TimeBarEffect;
    std::shared_ptr<Util::Text> m_SequenceText;
    std::shared_ptr<Util::GameObject> m_SequenceTextObject;

    // 計算倒數進度 (0.0 到 1.0)
    float CalculateProgress() const;

    // 狀態轉換
    void ChangeState(State newState);
};

#endif // ATTACK_HPP