#ifndef SKILL_HPP
#define SKILL_HPP

#include "Util/Animation.hpp"
#include "Effect/EffectManager.hpp"
#include "Util/Color.hpp"
#include "Object.hpp"

class Skill {
public:
    enum class State {
        IDLE,
        ACTIVE
    };

    Skill(int skillId, const std::vector<std::string>& imageSet, int duration = 175, float Cooldown = 2.0f);


    std::shared_ptr<Util::Animation> GetAnimation() const { return m_Animation; }
    void Play(const glm::vec2& position, float direction);

    // 檢查動畫是否已結束
    bool IsEnded() const;
    const std::vector<std::string>& GetImagePathSet() const { return m_ImagePathSet; }

    // 技能
    void Update(float deltaTime);
    State GetState() const { return m_State; }

    bool IsOnCooldown() const { return m_IsOnCooldown; }
    float GetRemainingCooldown() const { return m_CurrentCooldown; }
    void ResetCooldown() { m_IsOnCooldown = false; m_CurrentCooldown = 0.0f; }

private:
    std::vector<std::string> m_ImagePathSet;
    std::shared_ptr<Util::Animation> m_Animation;
    State m_State = State::IDLE;
    int m_Duration = 175;
    int m_SkillId = 1;

    // 特效參數
    float m_EffectRadius = 0.4f;
    Util::Color m_EffectColor = Util::Color::FromName(Util::Colors::WHITE);
    glm::vec2 m_EffectSize = {800, 800};
    std::shared_ptr<Effect::IEffect> m_CurrentEffect = nullptr;

    float m_Cooldown = 1.8f;       // 冷卻時間（秒）
    float m_CurrentCooldown = 0.0f; // 目前冷卻計時器
    bool m_IsOnCooldown = false;    // 檢查技能是否處於冷卻狀態的
};

#endif