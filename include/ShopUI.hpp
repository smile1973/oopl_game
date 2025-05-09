#ifndef SHOP_UI_HPP
#define SHOP_UI_HPP

#include "Util/GameObject.hpp"
#include "Character.hpp"
#include "Object.hpp"
#include "TextObject.hpp"
#include <memory>
#include <string>
#include <vector>

class ShopUI : public Util::GameObject {
public:
    // 建構子，接收要監控技能的角色參考
    explicit ShopUI(const std::shared_ptr<Character>& character);

    // 更新函式，用於刷新血條顯示
    void Update();

    [[nodiscard]] bool GetVisibility() const { return m_Visible; }
    [[nodiscard]] bool GetProduct() const { return m_Product; }

    [[nodiscard]] std::vector<std::shared_ptr<Util::GameObject>> GetChildren() const {
        std::vector<std::shared_ptr<Util::GameObject>> children;
        children.push_back(m_BaseIcon);
        return children;
    }

    void SetVisible(bool visible) override;

    void SwitchProduct(bool isLeft) ;

private:
    std::shared_ptr<Character> m_Character; // 我們正在監控的角色參考

    bool m_Visible = true;

    int m_Product = -1;

    std::shared_ptr<Object>m_BaseIcon;


    float baseX = -1;
    float baseY = -1;
};

#endif // SHOP_UI_HPP