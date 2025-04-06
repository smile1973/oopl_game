#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"
#include "Skill.hpp"
// #include "Enemy.hpp"

class Character : public Util::GameObject {
public:
    explicit Character(const std::vector<std::string>& ImageSet);
    enum class State {
        IDLE,
        USING_SKILL,
    };

    Character(const Character&) = delete;
    Character(Character&&) = delete;
    Character& operator=(const Character&) = delete;
    Character& operator=(Character&&) = delete;

    [[nodiscard]] const std::vector<std::string>& GetImagePathSet() const { return m_ImagePathSet; }
    [[nodiscard]] const glm::vec2& GetPosition() const { return m_Transform.translation; }
    [[nodiscard]] bool GetVisibility() const { return m_Visible; }
    // 檢測角色是否與另一個角色發生碰撞
    bool IfCollides(const std::shared_ptr<Character>& other, float Distance) const;
    bool IfCirclesCollide(const std::shared_ptr<Character>& other, float Distance) const;

    void SetPosition(const glm::vec2& Position) { m_Transform.translation = Position; }
    void SetInversion() { m_Transform.scale.x *= -1; } // 設定左右反轉角色

    void TowardNearestEnemy(const std::vector<std::shared_ptr<Character>>& m_Enemies); // 朝向最近的敵人

    // 技能
    void AddSkill(int skillId, const std::vector<std::string>& skillImageSet,
                 int duration = 175, float Cooldown = 2.0f);
    bool UseSkill(int skillId);  // 1=Z, 2=X, 3=C, 4=V
    virtual void Update();

    bool IsSkillOnCooldown(int skillId) const;
    [[nodiscard]] float GetRemainingCooldown(int skillId) const {
        auto it = m_Skills.find(skillId);
        if (it != m_Skills.end()) {
            return it->second->GetRemainingCooldown();
        }
        return 0.0f;
    }

    virtual void MovePosition(const glm::vec2& Position, float totalTime = 0.0f);  //平移位置
    virtual void MoveToPosition(const glm::vec2& targetPosition, float totalTime = 0.0f); //平移到某位置
    [[nodiscard]] virtual glm::vec2& GetTargetPosition() { return m_TargetPosition; }

private:
    void ResetPosition() { m_Transform.translation = {0, 0}; }
    void SwitchToIdle();
    void SwitchToSkill(int skillId);

    std::vector<std::string> m_ImagePathSet;
    std::shared_ptr<Util::Animation> m_IdleAnimation;

    // 存所有技能
    std::unordered_map<int, std::shared_ptr<Skill>> m_Skills;

    State m_State = State::IDLE;
    int m_CurrentSkillId = -1;
    std::shared_ptr<Skill> m_CurrentSkill = nullptr;


    bool m_IsMoving = false;
    float m_DistanceTraveled = 0.0f;
    float m_TotalTime = 0.0f;
    glm::vec2 m_TargetPosition = glm::vec2(0.0f, 0.0f);
    glm::vec2 m_MoveSpeed = glm::vec2(0.0f, 0.0f);
};


#endif //CHARACTER_HPP
