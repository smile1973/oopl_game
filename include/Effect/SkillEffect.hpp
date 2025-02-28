#ifndef SKILL_EFFECT_HPP
#define SKILL_EFFECT_HPP

#include "pch.hpp"
#include "Core/Drawable.hpp"
#include "Util/Transform.hpp"

namespace Util {
    class SkillEffect : public Core::Drawable {
    public:
        SkillEffect() = default;
        virtual ~SkillEffect() = default;

        virtual void Update(float deltaTime) = 0;
        virtual bool IsFinished() const = 0;
        virtual void Reset() = 0;
        virtual void Play() = 0;

    protected:
        bool m_IsPlaying = false;
        float m_Duration = 0.0f;
        float m_ElapsedTime = 0.0f;
    };
}

#endif // SKILL_EFFECT_HPP