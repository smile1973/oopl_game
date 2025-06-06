#ifndef CIRCLEATTACK_HPP
#define CIRCLEATTACK_HPP

#include "Attack/Attack.hpp"

class CircleAttack : public Attack {
public:
    CircleAttack(const glm::vec2& position, float delay, float radius = 100.0f, int sequenceNumber = 0);

    void SetRadius(float radius) { m_Radius = radius; }
    float GetRadius() const { return m_Radius; }

    void SetColor(const Util::Color& color) { m_Color = color; }

    void SetMovementParams(const glm::vec2& direction, float speed, float distance) {
        m_Direction = direction;
        m_Speed = speed;
        m_Distance = distance;
        m_IsMoving = true;
    }

    void CleanupVisuals() override;
    void SetAttackDuration(float duration) { m_AttackDuration = duration; }

    void SetAttackZIndex(float zIndex);

protected:
    void CreateWarningEffect() override;
    void CreateAttackEffect() override;
    bool CheckCollisionInternal(const std::shared_ptr<Character>& character) override;
    void SyncWithEffect() override;
    void OnCountdownStart() override;
    void OnAttackStart() override;

private:
    float m_Radius;
    Util::Color m_Color;
    bool m_UseGlowEffect = true;

    bool m_IsMoving = false;
    glm::vec2 m_Direction = {1.0f, 0.0f};
    float m_Speed = 200.0f;
    float m_Distance = 800.0f;

    static std::shared_ptr<Util::Image> s_ArrowImage;
    std::shared_ptr<Util::GameObject> m_DirectionIndicator;

    void CreateDirectionIndicator();
    float CalculateDirectionAngle() const;
};

#endif