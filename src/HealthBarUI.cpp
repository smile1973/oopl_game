#include "HealthBarUI.hpp"
#include "Util/Logger.hpp"
#include "Character.hpp" // 確保包含 Character.hpp

HealthBarUI::HealthBarUI(const std::shared_ptr<Character>& character)
    : m_Character(character) {

    m_Health = m_Character->GetHealth();

    constexpr float baseX = 0.0f; // 調整血條的初始 X 位置
    constexpr float baseY = -290.0f; // 調整血條的初始 Y 位置
    constexpr float spacing = 100.0f;

    for (int i = 0; i < MAX_HEALTH_BARS; ++i) {
        m_HealthBars.emplace_back(std::make_shared<Object>(GA_RESOURCE_DIR "/Image/UI/health_bar.png"));
        m_HealthBars[i]->SetPosition(glm::vec2(baseX + spacing*(i-1), baseY));
        m_HealthBars[i]->m_Transform.scale = {0.33,0.33};
        m_HealthBars[i]->SetZIndex(80);
        m_HealthBars[i]->SetVisible(false);
    }
}

void HealthBarUI::Update() {
    if (!m_Character) return;

    if (m_Visible != m_Character->GetVisibility()) {
        SetVisible(!m_Visible);
    }

    if (m_Health != m_Character->GetHealth()) {
        m_Health = m_Character->GetHealth();
        m_RemainingHealthBar--;
        if (m_RemainingHealthBar >= 0) {
            m_HealthBars[m_RemainingHealthBar]->SetVisible(false);
        }else {
            LOG_ERROR("Health Bar Visibility Error");
        }
    }
}

void HealthBarUI::SetVisible(const bool visible) {
    if (!m_Character) return;

    m_Visible = visible;

    for (int i = 0; i < MAX_HEALTH_BARS; ++i) {
        m_HealthBars[i]->SetVisible(m_Visible);
    }
}

void HealthBarUI::SetPosition(const glm::vec2& position) const {
    for (int i = 0; i < MAX_HEALTH_BARS; ++i) {
        constexpr float spacing = 65.0f;
        m_HealthBars[i]->SetPosition(position + glm::vec2(spacing*(i-1), 0));
    }
}

void HealthBarUI::Reset() {
    m_Health = 100;
    m_RemainingHealthBar = 3;

    m_Visible = false;
}

void HealthBarUI::FullHealthBar() {
    m_RemainingHealthBar = 3;
    for (int i = 0; i < MAX_HEALTH_BARS; ++i) {
        m_HealthBars[i]->SetVisible(true);
    }
}
