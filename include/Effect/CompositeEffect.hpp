#ifndef COMPOSITEEFFECT_HPP
#define COMPOSITEEFFECT_HPP

#include "Effect/Shape/CircleShape.hpp"
#include "Effect/Shape/EllipseShape.hpp"
#include "Effect/Shape/RectangleShape.hpp"
#include "Effect/Modifier/FillModifier.hpp"
#include "Effect/Modifier/EdgeModifier.hpp"
#include "Effect/Modifier/MovementModifier.hpp"


namespace Effect {
    class CompositeEffect : public IEffect {
    public:
        CompositeEffect(const std::shared_ptr<Shape::BaseShape>& baseShape);
        ~CompositeEffect() override = default;

        void Draw(const Core::Matrices& data) override;
        glm::vec2 GetSize() const override;

        void Update(float deltaTime) override;
        void Play(const glm::vec2& position, float zIndex = 0.0f) override;
        void Reset() override;

        void SetFillModifier(const Modifier::FillModifier& modifier) { m_FillModifier = modifier; }
        void SetEdgeModifier(const Modifier::EdgeModifier& modifier) { m_EdgeModifier = modifier; }
        void SetMovementModifier(const Modifier::MovementModifier& modifier) { m_MovementModifier = modifier; }

        void SetDuration(float duration) override {
            m_Duration = duration;
            if (m_BaseShape) {
                m_BaseShape->SetDuration(duration);
            }
        }

        std::shared_ptr<Shape::BaseShape> GetBaseShape() { return m_BaseShape; }
        void SetDirection(float direction) { m_direction = direction; }
        float GetDirection() { return m_direction; }

    private:
        std::shared_ptr<Shape::BaseShape> m_BaseShape;
        Modifier::FillModifier m_FillModifier;
        Modifier::EdgeModifier m_EdgeModifier;
        Modifier::MovementModifier m_MovementModifier;
        float m_direction = 1.0f;
    };
}

#endif