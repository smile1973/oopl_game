#ifndef LEVEL_UI_HPP
#define LEVEL_UI_HPP

#include "Util/GameObject.hpp"
#include "Character.hpp"
#include "Object.hpp"
#include "TextObject.hpp"
#include <memory>
#include <string>
#include <vector>

class LevelUI : public Util::GameObject {
public:
    // 建構子，接收要監控技能的角色參考
    explicit LevelUI(const std::shared_ptr<Character>& character);

    // 更新函式，用於刷新血條顯示
    void Update();

    [[nodiscard]] bool GetVisibility() const { return m_Visible; }

    [[nodiscard]] std::vector<std::shared_ptr<Util::GameObject>> GetChildren() const {
        std::vector<std::shared_ptr<Util::GameObject>> children;
        children.push_back(m_BaseIcon);
        children.push_back(m_ExperienceBar);
        children.push_back(m_Level);
        children.push_back(m_Money);
        return children;
    }

    void SetVisible(bool visible) override;

private:
    std::shared_ptr<Character> m_Character; // 我們正在監控的角色參考

    bool m_Visible = false;

    std::shared_ptr<Object>m_BaseIcon;
    std::shared_ptr<Object>m_ExperienceBar;
    std::shared_ptr<TextObject> m_Level;
    std::shared_ptr<TextObject> m_Money;
    int m_CurrentLevel = 0;
    int m_CurrentMoney = 0;
    int m_CurrentExperience = 0;

    float baseX = -1;
    float baseY = -1;
};

#endif // LEVEL_UI_HPP