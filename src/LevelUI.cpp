#include "LevelUI.hpp"
#include "Util/Logger.hpp"
#include "Character.hpp" // 確保包含 Character.hpp

LevelUI::LevelUI(const std::shared_ptr<Character>& character)
    : m_Character(character) {

    m_CurrentLevel = m_Character->GetLevel();
    m_CurrentMoney = m_Character->GetMoney();
    m_CurrentExperience = m_Character->GetExperience();

    // constexpr float baseX = 115.0f;
    // constexpr float baseY = -250.0f;
    baseX = 115.0f;
    baseY = -250.0f;

    m_BaseIcon = std::make_shared<Object>(GA_RESOURCE_DIR "/Image/UI/level_UI.png");
    m_BaseIcon -> SetPosition(glm::vec2(baseX, baseY));
    m_BaseIcon -> SetScale(0.9,0.9);
    m_BaseIcon -> SetZIndex(80);

    m_ExperienceBar = std::make_shared<Object>(GA_RESOURCE_DIR "/Image/UI/experience_bar.png");
    m_ExperienceBar -> SetPosition(glm::vec2(baseX-20, baseY-13));
    m_ExperienceBar -> SetScale(0.9,0.9);
    m_ExperienceBar -> SetZIndex(82);

    m_Level = std::make_shared<TextObject>("LL", 15);
    m_Level -> SetPosition(glm::vec2(baseX+24, baseY-15));
    m_Level -> SetZIndex(83);

    m_Money = std::make_shared<TextObject>("M", 15);
    m_Money -> SetPosition(glm::vec2(baseX+40, baseY+8));
    m_Money -> SetZIndex(83);

    LevelUI::SetVisible(false);
}

void LevelUI::Update() {
    if (!m_Character) return;

    if (m_Visible != m_Character->GetVisibility()) {
        SetVisible(!m_Visible);
    }

    if (m_CurrentLevel != m_Character->GetLevel()) {
        m_CurrentLevel = m_Character->GetLevel();
        m_Level -> SetText(std::to_string(m_CurrentLevel));
    }
    if (m_CurrentMoney != m_Character->GetMoney()) {
        m_CurrentMoney = m_Character->GetMoney();
        m_Money -> SetText(std::to_string(m_CurrentMoney));
    }
    if (m_CurrentExperience != m_Character->GetExperience()) {
        m_CurrentExperience = m_Character->GetExperience();
        const float size = 0.9f * m_CurrentExperience/100;
        m_ExperienceBar -> SetScale(size,0.9);
        const float X = baseX + 21.1 * m_CurrentExperience/100;
        m_ExperienceBar -> SetPosition(glm::vec2(X-40, baseY-12));
    }
}

void LevelUI::SetVisible(const bool visible) {
    if (!m_Character) return;

    m_Visible = visible;

    m_BaseIcon->SetVisible(m_Visible);
    m_ExperienceBar->SetVisible(m_Visible);
    if (m_Visible) {
        m_Level -> SetText(std::to_string(m_CurrentLevel));
        m_Money -> SetText(std::to_string(m_CurrentMoney));
    }else{
        m_Level -> SetText(" ");
        m_Money -> SetText(" ");
    }
}

