// Effect/CompositeEffect.cpp
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

        // 首先獲取程序ID
        Core::Program* program = nullptr;

        if (auto circleShape = dynamic_cast<Shape::CircleShape*>(m_BaseShape.get())) {
            (void) circleShape;
            program = Shape::CircleShape::GetProgram();
        } else if (auto ellipseShape = dynamic_cast<Shape::EllipseShape*>(m_BaseShape.get())) {
            (void) ellipseShape;
            program = Shape::EllipseShape::GetProgram();
        }

        if (!program) {
            LOG_ERROR("Failed to get program for CompositeEffect");
            return;
        }

        // 綁定程序
        program->Bind();

        // 應用所有修飾器
        m_FillModifier.Apply(*program);
        m_EdgeModifier.Apply(*program);
        m_AnimationModifier.Apply(*program, m_ElapsedTime);

        // 繪製基礎形狀
        m_BaseShape->Draw(data);
    }

    glm::vec2 CompositeEffect::GetSize() const {
        return m_BaseShape ? m_BaseShape->GetSize() : glm::vec2(0.0f);
    }

    void CompositeEffect::Update(float deltaTime) {
        if (m_State != State::ACTIVE) return;

        // 更新基礎形狀
        m_BaseShape->Update(deltaTime);

        // 應用移動修飾器
        if (m_MovementModifier.IsMoving()) {
            glm::vec2 position = m_Transform.translation;
            m_MovementModifier.Update(deltaTime, position);
            m_Transform.translation = position;

            // 如果到達終點，且基礎形狀還未結束，則結束特效
            if (m_MovementModifier.HasReachedDestination() && m_BaseShape->GetState() == State::ACTIVE) {
                m_BaseShape->Reset();
                m_State = State::FINISHED;
            }
        }

        // 同步狀態
        if (m_BaseShape->GetState() == State::FINISHED) {
            m_State = State::FINISHED;
        }

        // 更新時間
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

        // 同步位置到基礎形狀
        m_BaseShape->Play(position, zIndex);

        // 重置移動修飾器並設置起始位置
        if (m_MovementModifier.IsMoving()) {
            m_MovementModifier.Reset();
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

} // namespace Effect