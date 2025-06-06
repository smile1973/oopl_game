#ifndef IEFFECT_HPP
#define IEFFECT_HPP

#include "pch.hpp"
#include "Core/Drawable.hpp"
#include "Util/Transform.hpp"

namespace Effect {
    class IEffect : public Core::Drawable {
    public:
        enum class State {
            INACTIVE,
            ACTIVE,
            FINISHED
        };

        IEffect() = default;
        virtual ~IEffect() = default;

        virtual void Update(float deltaTime) = 0;
        virtual void Play(const glm::vec2& position, float zIndex) = 0;
        virtual void Reset() = 0;

        State GetState() const { return m_State; }
        bool IsFinished() const { return m_State == State::FINISHED; }
        bool IsActive() const { return m_State == State::ACTIVE; }

        const glm::vec2& GetPosition() const { return m_Transform.translation; }
        void SetPosition(const glm::vec2& position) { m_Transform.translation = position; }

        float GetDuration() const { return m_Duration; }
        virtual void SetDuration(float duration) { m_Duration = duration; }

        float GetZIndex() const { return m_ZIndex; }

    protected:
        State m_State = State::INACTIVE;
        Util::Transform m_Transform;
        float m_Duration = 1.0f;         // 特效持續時間（秒）
        float m_ElapsedTime = 0.0f;      // 已經過的時間（秒）
        float m_ZIndex = 0.0f;
    };
}

#endif