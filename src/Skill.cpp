#include "Skill.hpp"
#include "Util/Logger.hpp"
#include "Util/Time.hpp"
#include "Effect/EffectManager.hpp"

Skill::Skill(const std::vector<std::string>& imageSet, int duration, float effectRadius, const Util::Color& effectColor)
    : m_ImagePathSet(imageSet), m_Duration(duration), m_EffectRadius(effectRadius), m_EffectColor(effectColor) {

    // 創建技能動畫
    m_Animation = std::make_shared<Util::Animation>(imageSet, true, duration, false, 0);
}

void Skill::Play(const glm::vec2& position) {
    if (m_State == State::IDLE) {
        LOG_DEBUG("Playing skill animation and effect");
        m_State = State::ACTIVE;

        // 播放動畫
        if (m_Animation) {
            m_Animation->Play();
        }

        // 從效果管理器獲取並播放圓形特效
        auto effect = Effect::EffectManager::GetInstance().GetEffect(Effect::EffectType::CIRCLE);
        if (auto circleEffect = std::dynamic_pointer_cast<Effect::CircleEffect>(effect)) {
            circleEffect->SetRadius(m_EffectRadius);
            circleEffect->SetColor(m_EffectColor);
            circleEffect->SetDuration((static_cast<float>(m_Duration) / 1000.0f) * 1.5f); // 轉換為秒
            circleEffect->Play(position, 0.5f); // 設置特效位置和Z-index
            circleEffect->SetSize({800, 800});
            m_CurrentEffect = effect;
            LOG_DEBUG("Created effect at position: ({}, {}), radius: {}, duration: {}s",
                  position.x, position.y, m_EffectRadius,
                  static_cast<float>(m_Duration) / 1000.0f);
        }
    }
}

bool Skill::IsEnded() const {
    if (!m_Animation) return true;

    // 只檢查動畫是否結束，忽略特效
    return m_Animation->GetState() == Util::Animation::State::ENDED;
}

void Skill::Update(float deltaTime) {
    // 如果技能處於活躍狀態
    if (m_State == State::ACTIVE) {
        // 檢查是否結束
        if (IsEnded()) {
            m_State = State::IDLE;
            LOG_DEBUG("Skill ended");
            m_CurrentEffect = nullptr;
        }
    }
}