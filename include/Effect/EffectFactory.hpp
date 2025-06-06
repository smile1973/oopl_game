#ifndef EFFECT_FACTORY_HPP
#define EFFECT_FACTORY_HPP

#include "Effect/CompositeEffect.hpp"

namespace Effect {
    enum class EffectType {
        SKILL_Z,
        SKILL_X,
        SKILL_C,
        SKILL_V,

        ENEMY_ATTACK_2,

        RECT_LASER,
        RECT_BEAM,
    };

    class EffectFactory {
    public:
        static EffectFactory& GetInstance() {
            static EffectFactory instance;
            return instance;
        }

        static std::shared_ptr<CompositeEffect> CreateEffect(EffectType type);

    private:
        EffectFactory() = default;

        static std::shared_ptr<CompositeEffect> CreateCircleEffect(
            float radius,
            const Util::Color& color,
            float duration,
            Modifier::FillType fillType = Modifier::FillType::SOLID,
            Modifier::EdgeType edgeType = Modifier::EdgeType::NONE,
            const glm::vec2& size = {400, 400},
            float edgeWidth = 0.05f
        );

        static std::shared_ptr<CompositeEffect> CreateRectangleEffect(
            const glm::vec2& dimensions,
            const Util::Color& color,
            float duration,
            bool autoRotate = false,
            float rotationSpeed = 0.0f,
            const glm::vec2& size = {1600, 1600}
        );
    };
}

#endif