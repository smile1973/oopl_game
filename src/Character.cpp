#include "Character.hpp"
#include "Util/Image.hpp"
#include "Util/Renderer.hpp"
#include "Util/Logger.hpp"
#include "Util/Time.hpp"
#include "Util/TransformUtils.hpp"

Character::Character(const std::vector<std::string>& ImagePathSet) {
    // 建立閒置動畫
    m_IdleAnimation = std::make_shared<Util::Animation>(ImagePathSet, true, 250, true, 0);
    // 初始時設置為閒置動畫
    m_Drawable = m_IdleAnimation;
    m_ImagePathSet = ImagePathSet;
    ResetPosition();
}

void Character::AddSkill(int skillId, const std::vector<std::string>& skillImageSet,
                        int duration, float effectRadius, const Util::Color& effectColor) {
    // 創建並儲存新技能
    auto newSkill = std::make_shared<Skill>(skillImageSet, duration, effectRadius, effectColor);
    m_Skills[skillId] = newSkill;
    LOG_DEBUG("Added skill with ID: " + std::to_string(skillId));
}

void Character::UseSkill(int skillId) {
    if (m_State == State::IDLE) {
        // 檢查技能是否存在
        auto it = m_Skills.find(skillId);
        if (it != m_Skills.end()) {
            LOG_DEBUG("Character using skill with ID: " + std::to_string(skillId));
            SwitchToSkill(skillId);
        } else {
            LOG_DEBUG("Skill with ID " + std::to_string(skillId) + " not found!");
        }
    }
}

void Character::Update() {
    if (m_State == State::USING_SKILL && m_CurrentSkill) {
        // 更新技能
        m_CurrentSkill->Update(Util::Time::GetDeltaTimeMs() / 1000.0f);

        // 檢查技能是否結束
        if (m_CurrentSkill->IsEnded()) {
            SwitchToIdle();
        }
    }
}

void Character::UpdateAndDraw() {
    Update();

    // 然後繪製技能效果（如果當前正在使用技能）
    if (m_State == State::USING_SKILL && m_CurrentSkill) {
        // 創建一個新的變換矩陣，使效果在角色位置上顯示
        auto effectTransform = m_Transform;
        // 可能需要調整效果的位置，使其居中於角色

        auto data = Util::ConvertToUniformBufferData(
            effectTransform, m_CurrentSkill->GetEffect()->GetSize(), m_ZIndex - 0.1f);

        m_CurrentSkill->DrawEffect(data);

        // 添加偵錯資訊
        // LOG_DEBUG("Drawing effect at position: {}, Z-index: {}",
        //           m_Transform.translation, m_ZIndex + 1.0f);
    }
}

void Character::SwitchToIdle() {
    if (m_State != State::IDLE) {
        LOG_DEBUG("Switching to IDLE animation");
        m_State = State::IDLE;
        m_Drawable = m_IdleAnimation; // 直接切換到閒置動畫物件
        m_CurrentSkillId = -1;
        m_CurrentSkill = nullptr;
    }
}

void Character::SwitchToSkill(int skillId) {
    if (m_State == State::IDLE) {
        auto it = m_Skills.find(skillId);
        if (it != m_Skills.end()) {
            LOG_DEBUG("Switching to skill animation for skill ID: " + std::to_string(skillId));
            m_State = State::USING_SKILL;
            m_CurrentSkillId = skillId;
            m_CurrentSkill = it->second;

            // 播放技能
            m_CurrentSkill->Play();
            m_Drawable = m_CurrentSkill->GetAnimation();
        }
    }
}