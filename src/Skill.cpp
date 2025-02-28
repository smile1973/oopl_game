// Skill.cpp
#include "Skill.hpp"
#include "Util/Logger.hpp"
#include "Util/Time.hpp"

Skill::Skill(const std::vector<std::string>& imageSet, int duration, float effectRadius, const Util::Color& effectColor)
    : m_ImagePathSet(imageSet), m_Duration(duration) {
    
    // 創建技能動畫
    m_Animation = std::make_shared<Util::Animation>(imageSet, true, duration, false, 0);
    
    // 創建圓形效果
    m_Effect = std::make_shared<Util::CircleEffect>(effectRadius, static_cast<float>(duration) / 1000.0f, effectColor);
}

void Skill::Play() {
    if (m_State == State::IDLE) {
        LOG_DEBUG("Playing skill animation and effect");
        m_State = State::ACTIVE;
        
        // 播放動畫
        if (m_Animation) {
            m_Animation->Play();
        }
        
        // 播放效果
        if (m_Effect && m_UseEffect) {
            m_Effect->Play();
        }
    }
}

bool Skill::IsEnded() const {
    if (!m_Animation) return true;
    
    // 檢查動畫是否結束
    bool animEnded = m_Animation->GetState() == Util::Animation::State::ENDED;
    
    // 檢查效果是否結束
    bool effectEnded = !m_Effect || !m_UseEffect || m_Effect->IsFinished();
    
    return animEnded && effectEnded;
}

void Skill::Update(float deltaTime) {
    // 如果技能處於活躍狀態
    if (m_State == State::ACTIVE) {
        // 檢查是否結束
        if (IsEnded()) {
            m_State = State::IDLE;
            LOG_DEBUG("Skill ended");
        }
        
        // 更新效果
        if (m_Effect && m_UseEffect) {
            m_Effect->Update(deltaTime);
        }
    }
}

void Skill::DrawEffect(const Core::Matrices& data) {
    if (m_State == State::ACTIVE && m_Effect && m_UseEffect) {
        m_Effect->Draw(data);
    }
}