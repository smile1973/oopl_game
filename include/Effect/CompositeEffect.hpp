// include/Effect/CompositeEffect.hpp - 移除 AnimationModifier 版本
#ifndef COMPOSITEEFFECT_HPP
#define COMPOSITEEFFECT_HPP

#include "Effect/Shape/CircleShape.hpp"
#include "Effect/Shape/EllipseShape.hpp"
#include "Effect/Shape/RectangleShape.hpp"
#include "Effect/Modifier/FillModifier.hpp"
#include "Effect/Modifier/EdgeModifier.hpp"
#include "Effect/Modifier/MovementModifier.hpp"
// 移除 AnimationModifier 引用

namespace Effect {

    // 組合所有修飾器的特效類（移除動畫修飾器）
    class CompositeEffect : public IEffect {
    public:
        // 建構函數基於形狀類型
        CompositeEffect(const std::shared_ptr<Shape::BaseShape>& baseShape);
        ~CompositeEffect() override = default;

        // 實現自Core::Drawable
        void Draw(const Core::Matrices& data) override;
        glm::vec2 GetSize() const override;

        // 實現自Effect::IEffect
        void Update(float deltaTime) override;
        void Play(const glm::vec2& position, float zIndex = 0.0f) override;
        void Reset() override;

        // 修飾器設置（移除動畫修飾器）
        void SetFillModifier(const Modifier::FillModifier& modifier) { m_FillModifier = modifier; }
        void SetEdgeModifier(const Modifier::EdgeModifier& modifier) { m_EdgeModifier = modifier; }
        void SetMovementModifier(const Modifier::MovementModifier& modifier) { m_MovementModifier = modifier; }

        void SetDuration(float duration) override {
            m_Duration = duration;
            if (m_BaseShape) {
                m_BaseShape->SetDuration(duration);
            }
        }

        // 基礎形狀設置
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

} // namespace Effect

#endif //COMPOSITEEFFECT_HPP