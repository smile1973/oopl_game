#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>

#include "Util/GameObject.hpp"
// #include "Sprite.hpp"
#include "Util/Animation.hpp"

// 角色類別，繼承自遊戲物件 (GameObject)
class Character : public Util::GameObject {
public:
    // 使用一組圖片來初始化角色
    explicit Character(const std::vector<std::string>& ImageSet);
    // 角色的狀態枚舉類型
    enum class State {
        IDLE,  // 靜止狀態
        SKILL  // 施放技能狀態
    };
    // 設定技能動畫圖片
    void SetSkillImages(const std::vector<std::string>& SkillImageSet);

    // 禁止複製和移動建構、賦值
    Character(const Character&) = delete;
    Character(Character&&) = delete;
    Character& operator=(const Character&) = delete;
    Character& operator=(Character&&) = delete;

    // 獲取角色的圖片集
    [[nodiscard]] const std::vector<std::string>& GetImagePathSet() const { return m_ImagePathSet; }
    // 獲取角色當前位置
    [[nodiscard]] const glm::vec2& GetPosition() const { return m_Transform.translation; }
    // 獲取角色的可見狀態
    [[nodiscard]] bool GetVisibility() const { return m_Visible; }

    // 設定角色位置
    void SetPosition(const glm::vec2& Position) { m_Transform.translation = Position; }
    // 讓角色施放技能
    void UseSkill();
    // 更新角色狀態（例如切換動畫）
    void Update();

    // [[nodiscard]] bool IfCollides(const std::shared_ptr<Character>& other) const;
private:
    // 重置角色位置為原點
    void ResetPosition() { m_Transform.translation = {0, 0}; }
    // 切換回靜止動畫
    void SwitchToIdle();
    // 切換至技能動畫
    void SwitchToSkill();

    std::vector<std::string> m_ImagePathSet; // 角色的靜止狀態圖片集
    std::vector<std::string> m_SkillImagePathSet; // 角色的技能狀態圖片集

    std::shared_ptr<Util::Animation> m_IdleAnimation; // 角色靜止動畫
    std::shared_ptr<Util::Animation> m_SkillAnimation; // 角色技能動畫

    State m_State = State::IDLE; // 角色當前狀態
    // int m_SkillAnimationTime = 0;
    int m_SkillDuration = 500; // 技能動畫持續時間（毫秒）
};


#endif //CHARACTER_HPP
