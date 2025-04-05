#include "SkillUI.hpp"
#include "Util/Logger.hpp"

#include <sstream>
#include <iomanip>

SkillUI::SkillUI(const std::shared_ptr<Character>& character)
    : m_Character(character) {

    constexpr float baseX = -75.0f;
    constexpr float baseY = -250.0f;
    constexpr float spacing = 50.0f;

    for (int i = 0; i < NUM_SKILLS; ++i) {
        m_SkillIcons.emplace_back( std::make_shared<Object>(IconImagePath(i+1)) );
        m_SkillIcons[i] -> SetPosition(glm::vec2(baseX + i * spacing, baseY));
        m_SkillIcons[i] -> m_Transform.scale = {0.38f, 0.38f};
        m_SkillIcons[i] -> SetZIndex(80);

        m_CooldownTexts.emplace_back( std::make_shared<TextObject>(std::to_string(i+1) + "-0") );
        m_CooldownTexts[i] -> SetPosition(glm::vec2(baseX + i * spacing+5, baseY-5));
        LOG_INFO("CooldownTexts: {} -- {}", i, m_CooldownTexts[i]->GetPosition());
    }
    m_SkillIcons[1] -> MovePosition(glm::vec2(0,1.5));
}

void SkillUI::Update() {
    if (!m_Character) return;

    for (int i = 0; i < NUM_SKILLS; ++i) {
        int skillId = i + 1;
        float remainingCooldown = m_Character->GetRemainingCooldown(skillId);

        if (remainingCooldown > 0.5f) {
            std::stringstream ss;
            ss << std::fixed << std::setprecision(0) << remainingCooldown;
            m_CooldownTexts[i]->SetText(ss.str());
            LOG_INFO("CooldownTexts: {} -- {}", i, ss.str());
        } else {
            m_CooldownTexts[i]->SetText(" "); // 冷卻完畢，清除文字
        }
    }
}