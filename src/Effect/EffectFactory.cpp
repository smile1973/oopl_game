#include "Effect/EffectFactory.hpp"
#include "Util/Logger.hpp"
#include "Effect/Shape/RectangleShape.hpp"

namespace Effect {
    std::shared_ptr<CompositeEffect> EffectFactory::CreateCircleEffect(
        float radius,
        const Util::Color& color,
        float duration,
        Modifier::FillType fillType,
        Modifier::EdgeType edgeType,
        const glm::vec2& size,
        float edgeWidth) {

        auto circleShape = std::make_shared<Shape::CircleShape>(radius, duration);
        circleShape->SetColor(color);
        circleShape->SetSize(size);

        auto effect = std::make_shared<CompositeEffect>(circleShape);
        effect->SetFillModifier(Modifier::FillModifier(fillType, 0.02f));

        if (edgeType != Modifier::EdgeType::NONE) {
            Util::Color defaultEdgeColor = (edgeType == Modifier::EdgeType::GLOW) ?
                Util::Color(1.0f, 0.0f, 1.0f, 1.0f) : Util::Color(0.0f, 0.0f, 0.0f, 0.7f);
            effect->SetEdgeModifier(Modifier::EdgeModifier(edgeType, edgeWidth, defaultEdgeColor));
        }

        return effect;
    }

    std::shared_ptr<CompositeEffect> EffectFactory::CreateRectangleEffect(
        const glm::vec2& dimensions,
        const Util::Color& color,
        float duration,
        bool autoRotate,
        float rotationSpeed,
        const glm::vec2& size) {

        auto rectangleShape = std::make_shared<Shape::RectangleShape>(
            dimensions, 0.0f, 0.0f, duration, autoRotate, rotationSpeed
        );
        rectangleShape->SetColor(color);
        rectangleShape->SetSize(size);

        auto effect = std::make_shared<CompositeEffect>(rectangleShape);
        effect->SetFillModifier(Modifier::FillModifier(Modifier::FillType::SOLID));
        effect->SetEdgeModifier(Modifier::EdgeModifier(Modifier::EdgeType::GLOW, 0.02f, Util::Color(1.0f, 0.0f, 1.0f, 0.9f)));

        return effect;
    }

    std::shared_ptr<CompositeEffect> EffectFactory::CreateEffect(EffectType type) {
        std::shared_ptr<CompositeEffect> effect;

        switch (type) {
            case EffectType::SKILL_Z: {
                effect = CreateCircleEffect(
                    0.4f,
                    Util::Color(1.0f, 0.8f, 0.7f, 0.3f),
                    1.0f,
                    Modifier::FillType::SOLID,
                    Modifier::EdgeType::GLOW,
                    {500, 500},
                    0.005f
                );
                break;
            }

            case EffectType::SKILL_X: {
                effect = CreateCircleEffect(
                    0.4f,
                    Util::Color(1.0f, 0.8f, 0.7f, 0.1f),
                    2.0f,
                    Modifier::FillType::HOLLOW,
                    Modifier::EdgeType::GLOW,
                    {150, 150},
                    0.02f
                );
                effect->SetMovementModifier(Modifier::MovementModifier(true, 600.0f, 800.0f, {1.0f, 0.0f}));
                break;
            }

            case EffectType::SKILL_C: {
                auto ellipseShape = std::make_shared<Shape::EllipseShape>(glm::vec2(0.4f, 0.05f), 1.0f);
                ellipseShape->SetColor(Util::Color(1.0f, 1.0f, 1.0f, 0.05f));
                ellipseShape->SetSize({700, 700});
                effect = std::make_shared<CompositeEffect>(ellipseShape);
                effect->SetFillModifier(Modifier::FillModifier(Modifier::FillType::HOLLOW, 0.01f));
                effect->SetEdgeModifier(Modifier::EdgeModifier(Modifier::EdgeType::GLOW, 0.03f, Util::Color(1.0f, 1.0f, 1.0f, 0.7f)));
                break;
            }

            case EffectType::SKILL_V: {
                effect = CreateCircleEffect(
                    0.4f,
                    Util::Color(0.9f, 0.9f, 0.9f, 0.05f),
                    1.5f,
                    Modifier::FillType::SOLID,
                    Modifier::EdgeType::GLOW,
                    {400, 400},
                    0.08f
                );
                break;
            }

            case EffectType::ENEMY_ATTACK_2: {
                effect = CreateCircleEffect(
                    0.35f,
                    Util::Color(1.0f, 0.2f, 0.0f, 0.7f),
                    1.0f,
                    Modifier::FillType::HOLLOW,
                    Modifier::EdgeType::GLOW,
                    {400, 400},
                    0.06f
                );
                effect->SetEdgeModifier(Modifier::EdgeModifier(Modifier::EdgeType::GLOW, 0.06f, Util::Color(1.0f, 0.5f, 0.0f, 1.0f)));
                break;
            }

            case EffectType::RECT_LASER: {
                effect = CreateRectangleEffect(
                    glm::vec2(1.0f, 0.1f),
                    Util::Color(1.0f, 0.7f, 0.4f, 0.3f),
                    2.0f,
                    false,
                    0.0f,
                    {2560, 2560}
                );
                break;
            }

            case EffectType::RECT_BEAM: {
                effect = CreateRectangleEffect(
                    glm::vec2(1.0f, 0.05f),
                    Util::Color(1.0f, 0.7f, 0.4f, 0.3f),
                    5.0f,
                    true,
                    0.3f,
                    {1600, 1600}
                );
                break;
            }

            default:
                LOG_ERROR("Unknown effect type requested from EffectFactory");
                effect = CreateCircleEffect(
                    0.3f,
                    Util::Color(255, 0, 255, 128),
                    1.0f,
                    Modifier::FillType::SOLID,
                    Modifier::EdgeType::NONE,
                    {200, 200}
                );
                break;
        }

        return effect;
    }
}