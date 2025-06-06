#include "Effect/CompositeEffect.hpp"
#include "Util/Logger.hpp"

namespace Effect {
    CompositeEffect::CompositeEffect(const std::shared_ptr<Shape::BaseShape>& baseShape)
        : m_BaseShape(baseShape) {

        if (!baseShape) {
            LOG_ERROR("CompositeEffect created with null baseShape");
        }
    }

    void CompositeEffect::Draw(const Core::Matrices& data) {
        if (m_State != State::ACTIVE || !m_BaseShape) return;
        Core::Program* program = nullptr;

        if (auto circleShape = dynamic_cast<Shape::CircleShape*>(m_BaseShape.get())) {
            (void) circleShape;
            program = Shape::CircleShape::GetProgram();
        } else if (auto ellipseShape = dynamic_cast<Shape::EllipseShape*>(m_BaseShape.get())) {
            (void) ellipseShape;
            program = Shape::EllipseShape::GetProgram();
        } else if (auto rectangleShape = dynamic_cast<Shape::RectangleShape*>(m_BaseShape.get())) {
            (void) rectangleShape;
            program = Shape::RectangleShape::GetProgram();
        }

        if (!program) {
            LOG_ERROR("Failed to get program for CompositeEffect - BaseShape type not recognized");
            return;
        }

        program->Bind();

        m_FillModifier.Apply(*program);
        m_EdgeModifier.Apply(*program);

        // 時間uniform
        GLint timeLocation = glGetUniformLocation(program->GetId(), "u_Time");
        if (timeLocation != -1) {
            glUniform1f(timeLocation, m_ElapsedTime);
        }

        // 驗證
        program->Validate();
        m_BaseShape->Draw(data);
    }

    glm::vec2 CompositeEffect::GetSize() const {
        return m_BaseShape ? m_BaseShape->GetSize() : glm::vec2(0.0f);
    }

    void CompositeEffect::Update(float deltaTime) {
        if (m_State != State::ACTIVE) return;
        m_BaseShape->Update(deltaTime);

        // Apply movement modifier
        if (m_MovementModifier.IsMoving()) {
            glm::vec2 position = m_Transform.translation;
            m_MovementModifier.Update(deltaTime, position);
            m_Transform.translation = position;

            if (m_MovementModifier.HasReachedDestination() && m_BaseShape->GetState() == State::ACTIVE) {
                m_BaseShape->Reset();
                m_State = State::FINISHED;
            }
        }

        // Sync state
        if (m_BaseShape->GetState() == State::FINISHED) {
            m_State = State::FINISHED;
        }

        // Update time
        m_ElapsedTime += deltaTime;
        if (m_ElapsedTime >= m_Duration) {
            m_State = State::FINISHED;
        }
    }

    void CompositeEffect::Play(const glm::vec2& position, float zIndex) {
        if (!m_BaseShape) return;
        Reset();
        m_Transform.translation = position;
        m_ZIndex = zIndex;
        m_State = State::ACTIVE;

        m_BaseShape->Play(position, zIndex);

        if (m_MovementModifier.IsMoving()) {
            m_MovementModifier.Reset();
            m_MovementModifier.SetSpeed(m_MovementModifier.GetSpeed() * m_direction);
            m_MovementModifier.SetStartPosition(position);
        }
    }

    void CompositeEffect::Reset() {
        if (m_BaseShape) {
            m_BaseShape->Reset();
        }

        m_ElapsedTime = 0.0f;
        m_State = State::INACTIVE;
    }

}