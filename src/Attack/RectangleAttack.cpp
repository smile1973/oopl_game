#include "Attack/RectangleAttack.hpp"
#include "Effect/EffectManager.hpp"
#include "Util/Logger.hpp"
#include <cmath>
#include "App.hpp"

std::shared_ptr<Util::Image> RectangleAttack::s_ClockwiseImage = nullptr;
std::shared_ptr<Util::Image> RectangleAttack::s_CounterClockwiseImage = nullptr;

RectangleAttack::RectangleAttack(const glm::vec2& position, float delay,
                               float width, float height,
                               float rotation, int sequenceNumber)
    : Attack(position, delay, sequenceNumber),
      m_Width(width),
      m_Height(height),
      m_Rotation(rotation),
      m_Color(Util::Color::FromRGB(255, 50, 0, 150)) {}

RectangleAttack::RectangleAttack(const glm::vec2& position, float delay, Direction direction,
                               float width, float height, int sequenceNumber)
    : Attack(position, delay, sequenceNumber),
      m_Width(width),
      m_Height(height),
      m_Color(Util::Color::FromRGB(255, 20, 20, 200)),
      m_Direction(direction) {
    m_Rotation = CalculateRotationAngle();
    m_UseGlowEffect = true;
}

void RectangleAttack::CreateWarningEffect() {
    try {
        auto warningEffect = Effect::EffectManager::GetInstance().GetEffect(Effect::EffectType::RECT_BEAM);
        if (!warningEffect) return;

        if (auto rectangleShape = std::dynamic_pointer_cast<Effect::Shape::RectangleShape>(warningEffect->GetBaseShape())) {
            float maxDimension = std::max(m_Width, m_Height);
            float normalizedWidth = m_Width / maxDimension;
            float normalizedHeight = m_Height / maxDimension;

            rectangleShape->SetDimensions({normalizedWidth, normalizedHeight});
            rectangleShape->SetRotation(m_Rotation);
            rectangleShape->SetAutoRotation(false);
            rectangleShape->SetSize({maxDimension * 1.2f, maxDimension * 1.2f});
            rectangleShape->SetColor(Util::Color(0.9, 0.1, 0.1, 0.5));
        }

        warningEffect->SetFillModifier(Effect::Modifier::FillModifier(Effect::Modifier::FillType::SOLID));
        warningEffect->SetEdgeModifier(Effect::Modifier::EdgeModifier(
            Effect::Modifier::EdgeType::GLOW, 0.001, Util::Color(0.9, 0.1, 0.1, 0.7)
        ));

        warningEffect->SetDuration(m_Delay + 1.0f);
        warningEffect->Play(m_Position, GetWarningZIndex());

        m_WarningEffect = warningEffect;
    } catch (const std::exception& e) {
        LOG_ERROR("Exception in CreateWarningEffect: {}", e.what());
    }
}

void RectangleAttack::CreateAttackEffect() {
    try {
        auto rectangleEffect = Effect::EffectManager::GetInstance().GetEffect(Effect::EffectType::RECT_LASER);

        if (auto rectangleShape = std::dynamic_pointer_cast<Effect::Shape::RectangleShape>(rectangleEffect->GetBaseShape())) {
            float maxDimension = std::max(m_Width, m_Height);
            float normalizedWidth = m_Width / maxDimension;
            float normalizedHeight = m_Height / maxDimension;

            rectangleShape->SetDimensions({normalizedWidth, normalizedHeight});
            rectangleShape->SetRotation(m_Rotation);
            rectangleShape->SetAutoRotation(m_AutoRotate, m_RotationSpeed);
            rectangleShape->SetSize({maxDimension * 1.2f, maxDimension * 1.2f});
            rectangleShape->SetColor(Util::Color(0.9, 0.7, 0.3, 0.4));
        }

        rectangleEffect->SetFillModifier(Effect::Modifier::FillModifier(Effect::Modifier::FillType::SOLID));

        float effectDuration = m_AttackDuration * 1.5f;
        rectangleEffect->SetDuration(effectDuration);

        rectangleEffect->Play(m_Position, GetAttackZIndex());
        m_AttackEffect = rectangleEffect;
    } catch (const std::exception& e) {
        LOG_ERROR("Exception in CreateAttackEffect: {}", e.what());
    }
}

void RectangleAttack::CreateDirectionIndicator() {
    if (!s_ClockwiseImage) {
        s_ClockwiseImage = std::make_shared<Util::Image>(GA_RESOURCE_DIR "/Image/clockwise.png");
    }
    if (!s_CounterClockwiseImage) {
        s_CounterClockwiseImage = std::make_shared<Util::Image>(GA_RESOURCE_DIR "/Image/cclockwise.png");
    }

    if (!s_ClockwiseImage || !s_CounterClockwiseImage) {
        LOG_ERROR("Direction indicator images not available");
        return;
    }

    std::shared_ptr<Util::Image> directionImage =
        (m_RotationSpeed > 0) ? s_ClockwiseImage : s_CounterClockwiseImage;

    m_DirectionIndicator = std::make_shared<Util::GameObject>(
        directionImage,
        GetIndicatorZIndex(),
        m_Position,
        true
    );

    m_DirectionIndicator->m_Transform.scale = glm::vec2(1.0f, 1.0f);
    App::GetInstance().AddToRoot(m_DirectionIndicator);
    LOG_DEBUG("Direction indicator created for rotation speed: {}", m_RotationSpeed);
}

float RectangleAttack::CalculateRotationAngle() const {
    switch (m_Direction) {
        case Direction::HORIZONTAL:
            return 0.0f;
        case Direction::VERTICAL:
            return 1.5708f;
        case Direction::DIAGONAL_TL_BR:
            return 0.7854f;
        case Direction::DIAGONAL_TR_BL:
            return -0.7854f;
        default:
            return 0.0f;
    }
}

bool RectangleAttack::CheckCollisionInternal(const std::shared_ptr<Character>& character) {
    glm::vec2 characterPos = character->GetPosition();
    return IsPointInRectangle(characterPos);
}

bool RectangleAttack::IsPointInRectangle(const glm::vec2& circleCenter) const {
    float halfWidth = (m_Width * 1.2f) / 2.0f;
    float halfHeight = (m_Height * 1.2f) / 2.0f;

    glm::vec2 corners[4] = {
        {-halfWidth, -halfHeight},
        {halfWidth, -halfHeight},
        {halfWidth, halfHeight},
        {-halfWidth, halfHeight}
    };

    float cosA = cos(m_Rotation);
    float sinA = sin(m_Rotation);

    glm::vec2 rotatedCorners[4];
    for (int i = 0; i < 4; i++) {
        rotatedCorners[i].x = corners[i].x * cosA + corners[i].y * sinA + m_Position.x;
        rotatedCorners[i].y = -corners[i].x * sinA + corners[i].y * cosA + m_Position.y;
    }

    return IsPointInPolygon(circleCenter, rotatedCorners, 4);
}

bool RectangleAttack::IsPointInPolygon(const glm::vec2& point, const glm::vec2* vertices, int vertexCount) const{
    bool inside = false;
    for (int i = 0, j = vertexCount - 1; i < vertexCount; j = i++) {
        if (((vertices[i].y > point.y) != (vertices[j].y > point.y)) &&
            (point.x < (vertices[j].x - vertices[i].x) * (point.y - vertices[i].y) /
             (vertices[j].y - vertices[i].y) + vertices[i].x)) {
            inside = !inside;
        }
    }
    return inside;
}

void RectangleAttack::SyncWithEffect() {
    if (m_AttackEffect && m_AttackEffect->IsActive()) {
        auto rectangleShape = std::dynamic_pointer_cast<Effect::Shape::RectangleShape>(m_AttackEffect->GetBaseShape());
        if (rectangleShape) {
            float currentRotation = rectangleShape->GetRotation();
            m_Rotation = currentRotation;
        }
    }
}

void RectangleAttack::OnCountdownStart() {
    Attack::OnCountdownStart();

    if (m_AutoRotate) {
        CreateDirectionIndicator();
    }
}

void RectangleAttack::OnAttackStart() {
    Attack::OnAttackStart();

    if (m_DirectionIndicator) {
        App::GetInstance().RemoveFromRoot(m_DirectionIndicator);
        m_DirectionIndicator = nullptr;
    }
}

void RectangleAttack::OnCountdownUpdate(float deltaTime) {
    Attack::OnCountdownUpdate(deltaTime);

    if (m_DirectionIndicator && m_AutoRotate) {
        m_DirectionIndicator->m_Transform.rotation += deltaTime * m_RotationSpeed * -15.0f;
    }
}

void RectangleAttack::CleanupVisuals() {
    Attack::CleanupVisuals();
    if (m_DirectionIndicator) {
        App::GetInstance().RemoveFromRoot(m_DirectionIndicator);
        m_DirectionIndicator = nullptr;
    }
}

void RectangleAttack::SetAttackZIndex(float zIndex) {
    Attack::SetAttackZIndex(zIndex);
}