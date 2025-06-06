#ifndef RECTANGLEATTACK_HPP
#define RECTANGLEATTACK_HPP

#include "Attack/Attack.hpp"

class RectangleAttack : public Attack {
public:
    enum class Direction {
        HORIZONTAL,
        VERTICAL,
        DIAGONAL_TL_BR,
        DIAGONAL_TR_BL,
        CUSTOM
    };

    RectangleAttack(const glm::vec2& position, float delay,
                   float width = 200.0f, float height = 100.0f,
                   float rotation = 0.0f, int sequenceNumber = 0);

    RectangleAttack(const glm::vec2& position, float delay, Direction direction,
            float width = 80.0f, float length = 2000.0f, int sequenceNumber = 0);

    glm::vec2 GetSize() const { return {m_Width, m_Height}; }
    void SetSize(float width, float height) { m_Width = width; m_Height = height;}

    float GetRotation() const { return m_Rotation; }
    void SetRotation(float rotation);

    void SetColor(const Util::Color& color) { m_Color = color; }

    void SetAutoRotation(bool enable, float speed = 0.5f) {
        m_AutoRotate = enable;
        m_RotationSpeed = speed;
    }

    bool IsAutoRotating() const { return m_AutoRotate; }
    float GetRotationSpeed() const { return m_RotationSpeed; }
    void CleanupVisuals() override;

    void SetAttackZIndex(float zIndex);

protected:
    void CreateWarningEffect() override;
    void CreateAttackEffect() override;
    bool CheckCollisionInternal(const std::shared_ptr<Character>& character) override;
    void SyncWithEffect() override;
    void OnCountdownStart() override;
    void OnCountdownUpdate(float deltaTime) override;
    void OnAttackStart() override;

private:
    [[nodiscard]] float CalculateRotationAngle() const;
    [[nodiscard]] bool IsPointInPolygon(const glm::vec2& point, const glm::vec2* vertices, int vertexCount) const;
    [[nodiscard]] bool IsPointInRectangle(const glm::vec2& point) const;

    float m_Width;
    float m_Height;
    float m_Rotation;
    Util::Color m_Color;
    bool m_UseGlowEffect = true;
    Direction m_Direction;

    bool m_AutoRotate = false;
    float m_RotationSpeed = 0.5f;

    static std::shared_ptr<Util::Image> s_ClockwiseImage;
    static std::shared_ptr<Util::Image> s_CounterClockwiseImage;
    std::shared_ptr<Util::GameObject> m_DirectionIndicator;

    void CreateDirectionIndicator();
};

#endif