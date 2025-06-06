#include "Attack/CircleAttack.hpp"
#include "Effect/EffectManager.hpp"
#include "Util/Logger.hpp"
#include <cmath>
#include <App.hpp>

std::shared_ptr<Util::Image> CircleAttack::s_ArrowImage = nullptr;

CircleAttack::CircleAttack(const glm::vec2& position, float delay, float radius, int sequenceNumber)
    : Attack(position, delay, sequenceNumber),
      m_Radius(radius),
      m_Color(Util::Color(1.0, 1.0, 1.0, 0.3)) {
    m_AttackDuration = 0.5f;
}

void CircleAttack::CreateWarningEffect() {
    try {
        auto warningEffect = Effect::EffectManager::GetInstance().GetEffect(Effect::EffectType::ENEMY_ATTACK_2);

        if (auto circleShape = std::dynamic_pointer_cast<Effect::Shape::CircleShape>(warningEffect->GetBaseShape())) {
            float normalizedRadius = 0.35f;
            circleShape->SetRadius(normalizedRadius);

            float visualSize = m_Radius * 2.5f;
            circleShape->SetSize({visualSize, visualSize});

            circleShape->SetColor(Util::Color(1.0, 0.0, 0.0, 0.2));
            circleShape->SetDuration(m_Delay + 1.0f);
        }

        warningEffect->SetFillModifier(Effect::Modifier::FillModifier(Effect::Modifier::FillType::SOLID));
        warningEffect->SetEdgeModifier(Effect::Modifier::EdgeModifier(
            Effect::Modifier::EdgeType::GLOW, 0.005, Util::Color(1.0, 0.0, 0.0, 0.7)
        ));

        warningEffect->SetDuration(m_Delay + 1.0f);
        // 使用統一的 Z-index 管理
        warningEffect->Play(m_Position, GetWarningZIndex());

        m_WarningEffect = warningEffect;
    } catch (const std::exception& e) {
        LOG_ERROR("Exception in CreateWarningEffect: {}", e.what());
    }
}

void CircleAttack::CreateAttackEffect() {
    try {
        auto circleEffect = Effect::EffectManager::GetInstance().GetEffect(Effect::EffectType::ENEMY_ATTACK_2);

        if (auto circleShape = std::dynamic_pointer_cast<Effect::Shape::CircleShape>(circleEffect->GetBaseShape())) {
            float normalizedRadius = 0.35f;
            circleShape->SetRadius(normalizedRadius);

            float visualSize = m_Radius * 2.5f;
            circleShape->SetSize({visualSize, visualSize});

            circleShape->SetColor(m_Color);
        }

        circleEffect->SetFillModifier(Effect::Modifier::FillModifier(Effect::Modifier::FillType::SOLID));
        circleEffect->SetEdgeModifier(Effect::Modifier::EdgeModifier(
            Effect::Modifier::EdgeType::GLOW, 0.05, Util::Color(1.0, 0.0, 0.0, 0.7)
        ));

        if (m_IsMoving) {
            float moveDuration = m_Distance / m_Speed;
            m_AttackDuration = std::max(m_AttackDuration, moveDuration);

            LOG_DEBUG("Setting up movement with speed: {}, distance: {}, duration: {}",
                      m_Speed, m_Distance, moveDuration);

            auto movementMod = Effect::Modifier::MovementModifier(
                true, m_Speed, m_Distance, m_Direction
            );

            movementMod.SetStartPosition(m_Position);
            circleEffect->SetMovementModifier(movementMod);
        }

        circleEffect->SetDuration(m_AttackDuration);
        // 使用統一的 Z-index 管理，移除硬編碼的 z_ind
        circleEffect->Play(m_Position, GetAttackZIndex());
        m_AttackEffect = circleEffect;
    } catch (const std::exception& e) {
        LOG_ERROR("Exception in CreateAttackEffect: {}", e.what());
    }
}

bool CircleAttack::CheckCollisionInternal(const std::shared_ptr<Character>& character) {
    glm::vec2 characterPos = character->GetPosition();
    float distance = glm::length(characterPos - m_Position);
    return distance + 7.0f <= m_Radius;
}

void CircleAttack::SyncWithEffect() {
    if (m_AttackEffect && m_AttackEffect->IsActive()) {
        glm::vec2 effectPosition = m_AttackEffect->GetPosition();
        m_Position = effectPosition;
        m_Transform.translation = effectPosition;
    }
}

void CircleAttack::CreateDirectionIndicator() {
    if (!s_ArrowImage) {
        s_ArrowImage = std::make_shared<Util::Image>(GA_RESOURCE_DIR "/Image/arrow.png");
    }

    m_DirectionIndicator = std::make_shared<Util::GameObject>(
        s_ArrowImage,
        GetIndicatorZIndex(), // 使用統一的 Z-index 管理
        glm::vec2(0.0f, 0.0f),
        true
    );

    m_DirectionIndicator->m_Transform.translation = m_Position;
    m_DirectionIndicator->m_Transform.scale = glm::vec2(0.2f, 0.2f);

    float angle = atan2(m_Direction.y, m_Direction.x);
    m_DirectionIndicator->m_Transform.rotation = angle;

    App::GetInstance().AddToRoot(m_DirectionIndicator);
}

void CircleAttack::OnCountdownStart() {
    Attack::OnCountdownStart();

    if (m_IsMoving) {
        CreateDirectionIndicator();
    }
}

void CircleAttack::OnAttackStart() {
    Attack::OnAttackStart();

    if (m_DirectionIndicator) {
        App::GetInstance().RemoveFromRoot(m_DirectionIndicator);
        m_DirectionIndicator = nullptr;
    }
}

void CircleAttack::CleanupVisuals() {
    Attack::CleanupVisuals();
    if (m_DirectionIndicator) {
        App::GetInstance().RemoveFromRoot(m_DirectionIndicator);
        m_DirectionIndicator = nullptr;
    }
}

// 移除 SetZ 方法，改用繼承的 SetAttackZIndex
void CircleAttack::SetAttackZIndex(float zIndex) {
    Attack::SetAttackZIndex(zIndex); // 呼叫基類方法
}