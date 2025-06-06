#ifndef HEALTH_BAR_UI_HPP
#define HEALTH_BAR_UI_HPP

#include "Util/GameObject.hpp"
#include "Character.hpp"
#include "Object.hpp"
#include <memory>
#include <string>
#include <vector>

class HealthBarUI : public Util::GameObject {
public:
    // 建構子，接收要監控技能的角色參考
    explicit HealthBarUI(const std::shared_ptr<Character>& character);

    // 更新函式，用於刷新血條顯示
    void Update();
    void Reset();
    void FullHealthBar();

    [[nodiscard]] bool GetVisibility() const { return m_Visible; }
    [[nodiscard]] int GetHealthBar() const { return m_RemainingHealthBar; }

    [[nodiscard]] std::vector<std::shared_ptr<Util::GameObject>> GetChildren() const {
        std::vector<std::shared_ptr<Util::GameObject>> children;
        for (const auto& bar : m_HealthBars) {
            children.push_back(bar);
        }
        return children;
    }

    void SetVisible(bool visible) override;
    void SetPosition(const glm::vec2& position) const;

private:
    std::shared_ptr<Character> m_Character; // 我們正在監控的角色參考

    int m_Health = 100;
    int m_RemainingHealthBar = 3;
    bool m_Visible = false;

    static constexpr int MAX_HEALTH_BARS = 3;

    std::vector<std::shared_ptr<Object>> m_HealthBars; // 血條圖片
};

#endif // HEALTH_BAR_UI_HPP