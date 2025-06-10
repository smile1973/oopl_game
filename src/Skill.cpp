#include "Skill.hpp"
#include "Util/Logger.hpp"
#include "Util/Time.hpp"
#include "Effect/EffectManager.hpp"
#include "Effect/CompositeEffect.hpp"

Skill::Skill(int skillId, const std::vector<std::string>& imageSet, int duration, float Cooldown)
        : m_ImagePathSet(imageSet), m_Duration(duration), m_SkillId(skillId), m_Cooldown(Cooldown) {

    m_Animation = std::make_shared<Util::Animation>(imageSet, true, duration, false, 0);
}

void Skill::Play(const glm::vec2& position, float direction) {
    if (m_State == State::IDLE && !m_IsOnCooldown) {
        m_State = State::ACTIVE;

        // 開始冷卻
        m_IsOnCooldown = true;
        m_CurrentCooldown = m_Cooldown;

        if (m_Animation) {
            m_Animation->Play();
        }

        Effect::EffectType effectType;
        switch (m_SkillId) {
            case 1: effectType = Effect::EffectType::SKILL_Z; break;
            case 2: effectType = Effect::EffectType::SKILL_X; break;
            case 3: effectType = Effect::EffectType::SKILL_C; break;
            case 4: effectType = Effect::EffectType::SKILL_V; break;
            default: effectType = Effect::EffectType::SKILL_Z; break;
        }

        auto effect = Effect::EffectManager::GetInstance().GetEffect(effectType);
        effect->SetDuration(static_cast<float>(m_Duration) / 300.0f);
        if (m_SkillId == 2) effect->SetDirection(direction);
        effect->Play(position, 45.0f);
        m_CurrentEffect = effect;
    } else if (m_IsOnCooldown) {
        LOG_DEBUG("Skill {} is on cooldown for {:.1f} seconds",
                 m_SkillId, m_CurrentCooldown);
    }
}

bool Skill::IsEnded() const {
    if (!m_Animation) return true;
    return m_Animation->GetState() == Util::Animation::State::ENDED;
}

void Skill::Update(float deltaTime) {
    // 更新冷卻計時器
    if (m_IsOnCooldown) {
        m_CurrentCooldown -= deltaTime;
        if (m_CurrentCooldown <= 0.0f) {
            m_IsOnCooldown = false;
            m_CurrentCooldown = 0.0f;
            // LOG_DEBUG("Skill {} cooldown finished", m_SkillId);
        }
    }
    (void)deltaTime;
    if (m_State == State::ACTIVE) {
        if (IsEnded()) {
            m_State = State::IDLE;
            m_CurrentEffect = nullptr;
            // LOG_DEBUG("Skill ended");
        }
    }
}