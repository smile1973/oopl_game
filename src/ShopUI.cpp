#include "ShopUI.hpp"
#include "Util/Logger.hpp"
#include "Character.hpp" // 確保包含 Character.hpp

ShopUI::ShopUI(const std::shared_ptr<Character>& character)
    : m_Character(character) {

    baseX = 0.0f;
    baseY = 100.0f;

    m_BaseIcon = std::make_shared<Object>(GA_RESOURCE_DIR "/Image/UI/shop_UI.png");
    m_BaseIcon -> SetPosition(glm::vec2(baseX, baseY));
    m_BaseIcon -> SetScale(1.098,1.098);
    m_BaseIcon -> SetZIndex(95);

    ShopUI::SetVisible(false);
}

void ShopUI::SetVisible(const bool visible) {
    if (!m_Character) return;

    m_Visible = visible;

    m_BaseIcon->SetVisible(m_Visible);
}

void ShopUI::SwitchProduct(const bool isLeft) {
    if (!m_Character) return;
    if (isLeft) {
        m_Product=0;
        m_BaseIcon->SetImage(GA_RESOURCE_DIR "/Image/UI/shop_UI_left.png");
    }else{
        m_Product=1;
        m_BaseIcon->SetImage(GA_RESOURCE_DIR "/Image/UI/shop_UI_right2.png");
    }
}

