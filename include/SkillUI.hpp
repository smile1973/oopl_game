#ifndef SKILL_UI_HPP
#define SKILL_UI_HPP

#include "Util/Text.hpp"
#include "Util/GameObject.hpp"
#include "Character.hpp"
#include "Object.hpp"
#include <memory>
#include <string>
#include <vector>

class SkillUI : public Util::GameObject {
public:
    // 建構子，接收要監控技能的角色參考
    explicit SkillUI(const std::shared_ptr<Character>& character);

    // 更新函式，用於刷新冷卻顯示
    void Update();

    [[nodiscard]] std::vector<std::shared_ptr<Util::GameObject>> GetChildren() const {
        std::vector<std::shared_ptr<Util::GameObject>> children;
        for (const auto& icon : m_SkillIcons) {
            children.push_back(icon);
        }
        return children;
    }

private:
    std::shared_ptr<Character> m_Character; // 我們正在監控技能的角色參考

    static constexpr int NUM_SKILLS = 4; // 要顯示的技能數量
    static constexpr int TEXT_OFFSET_Y = 20; // 冷卻文字在圖示下方的偏移量
    static constexpr auto FONT_PATH = GA_RESOURCE_DIR "/Font/Inkfree.ttf"; // 預設字型路徑
    static constexpr int FONT_SIZE = 16; // 預設字型大小

    std::vector<std::shared_ptr<Object>> m_SkillIcons; // 技能圖示圖片
    std::vector<std::shared_ptr<Util::Text>> m_CooldownTexts; // 技能冷卻文字

    static std::string IconImagePath(const int skillId) {
        switch (skillId) {
            case 1: return GA_RESOURCE_DIR "/Image/UI/skill_z_icon.png";
            case 2: return GA_RESOURCE_DIR "/Image/UI/skill_x_icon.png";
            case 3: return GA_RESOURCE_DIR "/Image/UI/skill_c_icon.png";
            case 4: return GA_RESOURCE_DIR "/Image/UI/skill_v_icon.png";
            default: return GA_RESOURCE_DIR "/Image/UI/skill_z_icon.png";
        }
    }
};

#endif // SKILL_UI_HPP