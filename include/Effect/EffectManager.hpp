#ifndef EFFECT_MANAGER_HPP
#define EFFECT_MANAGER_HPP

#include <queue>
#include "Effect/CompositeEffect.hpp"
#include "Effect/EffectFactory.hpp"
#include "Util/GameObject.hpp"
#include "Util/Logger.hpp"
#include "Util/TransformUtils.hpp"

namespace Effect {
    class EffectManager : public Util::GameObject {
    public:
        static EffectManager& GetInstance() {
            static EffectManager instance;
            return instance;
        }

        void Initialize(size_t initialPoolSize = 10);
        std::shared_ptr<CompositeEffect> GetEffect(EffectType type);

        void Update(float deltaTime);
        void Draw() override;
        std::shared_ptr<CompositeEffect> PlayEffect(
            EffectType type,
            const glm::vec2& position,
            float zIndex = 0.0f,
            float duration = 1.0f
        );

        size_t GetActiveEffectsCount() const { return m_ActiveEffects.size(); }
        void ClearAllEffects() {
            for (auto& effect : m_ActiveEffects) {
                if (effect) {
                    effect->Reset();
                }
            }
            m_ActiveEffects.clear();
        }

    private:
        EffectManager() : Util::GameObject(nullptr, 30.0f) {}
        std::unordered_map<EffectType, std::queue<std::shared_ptr<CompositeEffect>>> m_InactiveEffects;
        std::vector<std::shared_ptr<CompositeEffect>> m_ActiveEffects;
    };
}

#endif