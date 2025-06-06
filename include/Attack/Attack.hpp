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

    static constexpr float ZINDEX_WARNING_OFFSET = -2.0f;      // 警告偏移
    static constexpr float ZINDEX_ATTACK_OFFSET = 0.0f;        // 攻擊偏移
    static constexpr float ZINDEX_TIMEBAR_OFFSET = 1.0f;       // 時間條偏移
    static constexpr float ZINDEX_INDICATOR_OFFSET = 2.0f;     // 方向指示器偏移

    Attack(const glm::vec2& position, float delay, int sequenceNumber = 0);
    virtual ~Attack() = default;

    void Update(float deltaTime);
    void Draw() override;

    [[nodiscard]] bool IsFinished() const { return m_State == State::FINISHED; }
    State GetState() const { return m_State; }

    bool CheckCollision(const std::shared_ptr<Character>& character);

    void SetPosition(const glm::vec2& position);
    void SetDelay(float delay) { m_Delay = delay; }
    void SetTargetCharacter(std::shared_ptr<Character> target) { m_TargetCharacter = target; }

    void SetAttackZIndex(float zIndex) {
        SetZIndex(zIndex);
        UpdateAllEffectZIndexes();
    }

    float GetWarningZIndex() const { return GetZIndex() + ZINDEX_WARNING_OFFSET; }
    float GetAttackZIndex() const { return GetZIndex() + ZINDEX_ATTACK_OFFSET; }
    float GetTimeBarZIndex() const { return GetZIndex() + ZINDEX_TIMEBAR_OFFSET; }
    float GetIndicatorZIndex() const { return GetZIndex() + ZINDEX_INDICATOR_OFFSET; }

    int GetSequenceNumber() const { return m_SequenceNumber; }
    float GetDelay() const { return m_Delay; }
    float GetElapsedTime() const { return m_ElapsedTime; }
    const glm::vec2& GetAttackPosition() const { return m_Position; }

    void SetAttackDuration(float duration) { m_AttackDuration = duration; }
    float GetAttackDuration() const { return m_AttackDuration; }

    [[nodiscard]] std::shared_ptr<Effect::CompositeEffect> GetWarningEffect() const { return m_WarningEffect; }
    [[nodiscard]] std::shared_ptr<Effect::CompositeEffect> GetAttackEffect() const { return m_AttackEffect; }
    virtual void CleanupVisuals() {};

protected:
    virtual void OnWarningStart();
    virtual void OnWarningUpdate(float deltaTime);
    virtual void OnCountdownStart();
    virtual void OnCountdownUpdate(float deltaTime);
    virtual void OnAttackStart();
    virtual void OnAttackUpdate(float deltaTime);
    virtual void OnFinishedStart();

    virtual void CreateWarningEffect() = 0;
    virtual void CreateAttackEffect() = 0;
    virtual void CreateTimeBar();
    virtual void UpdateTimeBar(float progress);

    virtual bool CheckCollisionInternal(const std::shared_ptr<Character>& character) = 0;
    virtual void SyncWithEffect() {}

    virtual void UpdateAllEffectZIndexes();

    State m_State = State::CREATED;
    bool m_IsFirstUpdate = true;
    glm::vec2 m_Position;
    float m_Delay;
    float m_ElapsedTime = 0.0f;
    int m_SequenceNumber;
    float m_AttackDuration = 0.5f;

    std::shared_ptr<Character> m_TargetCharacter = nullptr;

    std::shared_ptr<Effect::CompositeEffect> m_WarningEffect;
    std::shared_ptr<Effect::CompositeEffect> m_AttackEffect;
    std::shared_ptr<Effect::CompositeEffect> m_TimeBarEffect;
    std::shared_ptr<Util::Text> m_SequenceText;
    std::shared_ptr<Util::GameObject> m_SequenceTextObject;

    // 倒數進度 (0 ~ 1)
    float CalculateProgress() const;

    void ChangeState(State newState);
};

#endif