#ifndef MOVEMENTMODIFIER_HPP
#define MOVEMENTMODIFIER_HPP

#include "Effect/Shape/BaseShape.hpp"

namespace Effect {
    namespace Modifier {

        class MovementModifier {
        public:
            MovementModifier(bool isMoving = false, float speed = 200.0f, float distance = 400.0f, const glm::vec2& direction = {1.0f, 0.0f});

            void Update(float deltaTime, glm::vec2& position);

            void SetMoving(bool isMoving) { m_IsMoving = isMoving; }
            bool IsMoving() const { return m_IsMoving; }

            void SetSpeed(float speed) { m_Speed = speed; }
            float GetSpeed() const { return m_Speed; }

            void SetDistance(float distance) { m_MaxDistance = distance; }
            float GetDistance() const { return m_MaxDistance; }

            void SetDirection(const glm::vec2& direction) { m_Direction = glm::normalize(direction); }
            const glm::vec2& GetDirection() const { return m_Direction; }

            void SetStartPosition(const glm::vec2& position) { m_StartPosition = position; }
            void Reset();

            bool HasReachedDestination() const;

        private:
            bool m_IsMoving;
            float m_Speed;
            float m_MaxDistance;
            glm::vec2 m_Direction;

            glm::vec2 m_StartPosition;
            float m_DistanceTraveled = 0.0f;
        };

    }
}

#endif
