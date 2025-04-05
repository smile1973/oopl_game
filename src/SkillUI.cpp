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
        m_SkillIcons[i] -> SetZIndex(60);

        m_CooldownTexts.emplace_back( std::make_shared<Util::Text>(FONT_PATH, FONT_SIZE, "---", Util::Color(255, 2, 2)) );
        // m_CooldownTexts[i]->
    }
    m_SkillIcons[1] -> MovePosition(glm::vec2(0,1.5));

}

void SkillUI::Update() {
    if (!m_Character) {
        return;
    }

    for (int i = 0; i < NUM_SKILLS; ++i) {
        int skillId = i + 1;
        float remainingCooldown = m_Character->GetRemainingCooldown(skillId);

        if (remainingCooldown > 0.0f) {
            std::stringstream ss;
            ss << std::fixed << std::setprecision(1) << remainingCooldown;
            m_CooldownTexts[i]->SetText(ss.str());
        } else {
            m_CooldownTexts[i]->SetText(""); // 冷卻完畢，清除文字
        }
    }
}