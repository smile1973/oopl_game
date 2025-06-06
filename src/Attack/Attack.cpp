#include "Attack/Attack.hpp"
#include "Util/Logger.hpp"
#include "Util/Time.hpp"
#include "Effect/EffectFactory.hpp"
#include "Effect/EffectManager.hpp"

Attack::Attack(const glm::vec2& position, float delay, int sequenceNumber)
    : Util::GameObject(nullptr, 20.0f),
      m_Position(position),
      m_Delay(delay),
      m_SequenceNumber(sequenceNumber){

    // 初始化
    m_Transform.translation = position;
    m_State = State::CREATED;
}

void Attack::Update(float deltaTime) {
    if (m_IsFirstUpdate) {
        m_IsFirstUpdate = false;
        ChangeState(State::WARNING);
        return;
    }

    m_ElapsedTime += deltaTime;

    switch (m_State) {
        case State::WARNING:
            OnWarningUpdate(deltaTime);
            if (m_ElapsedTime >= 0.5f) {
                ChangeState(State::COUNTDOWN);
            }
            break;

        case State::COUNTDOWN:
            OnCountdownUpdate(deltaTime);
            UpdateTimeBar(CalculateProgress());
            if (m_ElapsedTime >= m_Delay) {
                ChangeState(State::ATTACKING);
            }
            break;

        case State::ATTACKING:
            OnAttackUpdate(deltaTime);
            if (m_ElapsedTime >= m_AttackDuration) {
                ChangeState(State::FINISHED);
            }
            break;

        case State::FINISHED:
        case State::CREATED:
            break;
    }
}

void Attack::Draw() {
    if (m_State == State::CREATED) {
        return;
    }
    Util::GameObject::Draw();
}

void Attack::SetPosition(const glm::vec2& position) {
    m_Position = position;
    m_Transform.translation = position;

    // 更新所有特效位置
    if (m_WarningEffect && m_WarningEffect->IsActive()) {
        m_WarningEffect->Play(position, GetWarningZIndex());
    }
    if (m_AttackEffect && m_AttackEffect->IsActive()) {
        m_AttackEffect->Play(position, GetAttackZIndex());
    }
    if (m_TimeBarEffect && m_TimeBarEffect->IsActive()) {
        glm::vec2 barPosition = position;
        barPosition.y -= 50.0f;
        m_TimeBarEffect->Play(barPosition, GetTimeBarZIndex());
    }

    if (m_SequenceText) {
        m_SequenceText->SetText(std::to_string(m_SequenceNumber));
    }
}

void Attack::UpdateAllEffectZIndexes() {
    if (m_WarningEffect && m_WarningEffect->IsActive()) {
        m_WarningEffect->Play(m_Position, GetWarningZIndex());
    }
    if (m_AttackEffect && m_AttackEffect->IsActive()) {
        m_AttackEffect->Play(m_Position, GetAttackZIndex());
    }
    if (m_TimeBarEffect && m_TimeBarEffect->IsActive()) {
        glm::vec2 barPosition = m_Position;
        barPosition.y -= 50.0f;
        m_TimeBarEffect->Play(barPosition, GetTimeBarZIndex());
    }
}

void Attack::ChangeState(State newState) {
    if (m_State == newState) return;

    m_State = newState;
    m_ElapsedTime = 0.0f;

    switch (newState) {
        case State::WARNING:
            OnWarningStart();
            break;
        case State::COUNTDOWN:
            OnCountdownStart();
            break;
        case State::ATTACKING:
            OnAttackStart();
            break;
        case State::FINISHED:
            OnFinishedStart();
            break;
        case State::CREATED:
            break;
    }
}

void Attack::OnWarningStart() {
    CreateWarningEffect();
}

void Attack::OnWarningUpdate(float deltaTime) {
    (void)deltaTime;
}

void Attack::OnCountdownStart() {
    CreateTimeBar();
}

void Attack::OnCountdownUpdate(float deltaTime) {
    (void)deltaTime;
}

void Attack::OnAttackStart() {
    CreateAttackEffect();

    if (m_WarningEffect) {
        m_WarningEffect->Reset();
    }
    if (m_TimeBarEffect) {
        m_TimeBarEffect->Reset();
    }
    CleanupVisuals();
}

void Attack::OnAttackUpdate(float deltaTime) {
    (void) deltaTime;
    SyncWithEffect();
    if (m_TargetCharacter) {
        CheckCollision(m_TargetCharacter);
    }

    if (m_AttackEffect && m_AttackEffect->IsFinished()) {
        CreateAttackEffect();
    }
}

void Attack::OnFinishedStart() {
    if (m_AttackEffect) {
        m_AttackEffect->Reset();
    }
}

void Attack::CreateTimeBar() {
    auto rectangleEffect = Effect::EffectManager::GetInstance().GetEffect(Effect::EffectType::RECT_BEAM);

    if (auto rectangleShape = std::dynamic_pointer_cast<Effect::Shape::RectangleShape>(rectangleEffect->GetBaseShape())) {
        rectangleShape->SetDimensions(glm::vec2(1.0f, 0.1f));
        rectangleShape->SetSize({200.0, 200.0});
        rectangleShape->SetRotation(0.0f);
        rectangleShape->SetAutoRotation(false);
    }

    rectangleEffect->SetFillModifier(Effect::Modifier::FillModifier(Effect::Modifier::FillType::SOLID));
    rectangleEffect->SetEdgeModifier(Effect::Modifier::EdgeModifier(
        Effect::Modifier::EdgeType::GLOW,
        0.005f,
        Util::Color::FromName(Util::Colors::WHITE)
    ));

    rectangleEffect->GetBaseShape()->SetColor(Util::Color(0.9, 0.9, 0.9, 0.5));

    glm::vec2 barPosition = m_Position;
    barPosition.y -= 50.0f;

    rectangleEffect->SetDuration(m_Delay);
    rectangleEffect->Play(barPosition, GetTimeBarZIndex());

    m_TimeBarEffect = rectangleEffect;
}

void Attack::UpdateTimeBar(float progress) {
    if (!m_TimeBarEffect) return;
    if (auto rectangleShape = std::dynamic_pointer_cast<Effect::Shape::RectangleShape>(m_TimeBarEffect->GetBaseShape())) {
        float width = 0.8f * (1.0f - progress);
        rectangleShape->SetDimensions(glm::vec2(width, 0.05f));
    }
}

float Attack::CalculateProgress() const {
    if (m_Delay <= 0.0f) return 1.0f;
    return m_ElapsedTime / m_Delay;
}

bool Attack::CheckCollision(const std::shared_ptr<Character>& character) {
    if (m_State != State::ATTACKING) return false;

    if (character->IsInvincible()) {
        return false;
    }

    if (CheckCollisionInternal(character)) {
        character->TakeDamage(1);
        return true;
    }

    return false;
}