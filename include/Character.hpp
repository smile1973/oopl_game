#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>

#include "Util/GameObject.hpp"
// #include "Sprite.hpp"
#include "Util/Animation.hpp"

class Character : public Util::GameObject {
public:
    explicit Character(const std::vector<std::string>& ImageSet);
    enum class State {
        IDLE,
        SKILL
    };
    void SetSkillImages(const std::vector<std::string>& SkillImageSet);

    Character(const Character&) = delete;
    Character(Character&&) = delete;
    Character& operator=(const Character&) = delete;
    Character& operator=(Character&&) = delete;

    [[nodiscard]] const std::vector<std::string>& GetImagePathSet() const { return m_ImagePathSet; }
    [[nodiscard]] const glm::vec2& GetPosition() const { return m_Transform.translation; }
    [[nodiscard]] bool GetVisibility() const { return m_Visible; }

    void SetPosition(const glm::vec2& Position) { m_Transform.translation = Position; }
    void UseSkill();
    void Update();

    // [[nodiscard]] bool IfCollides(const std::shared_ptr<Character>& other) const;
private:
    void ResetPosition() { m_Transform.translation = {0, 0}; }
    void SwitchToIdle();
    void SwitchToSkill();

    std::vector<std::string> m_ImagePathSet;
    std::vector<std::string> m_SkillImagePathSet;
    std::shared_ptr<Util::Animation> m_IdleAnimation;
    std::shared_ptr<Util::Animation> m_SkillAnimation;
    State m_State = State::IDLE;
    // int m_SkillAnimationTime = 0;
    int m_SkillDuration = 500; // 技能動畫持續時間（毫秒）
};


#endif //CHARACTER_HPP
