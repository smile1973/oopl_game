#include "Effect/EffectManager.hpp"
#include "Util/TransformUtils.hpp"
#include "Util/Logger.hpp"

namespace Effect {

    void EffectManager::Initialize(size_t initialPoolSize) {
        EffectType initOrder[] = {
            EffectType::SKILL_Z,
            EffectType::SKILL_X,
            EffectType::SKILL_C,
            EffectType::SKILL_V,
            EffectType::ENEMY_ATTACK_2,
            EffectType::RECT_LASER,
            EffectType::RECT_BEAM,
        };
        
        for (auto type : initOrder) {
            for (size_t j = 0; j < initialPoolSize; ++j) {
                auto effect = EffectFactory::GetInstance().CreateEffect(type);
                effect->GetBaseShape()->SetUserData(static_cast<int>(type));

                m_InactiveEffects[type].push(effect);
            }
        }
        LOG_INFO("EffectManager initialized with {} effects per type", initialPoolSize);
    }

    std::shared_ptr<CompositeEffect> EffectManager::GetEffect(EffectType type) {
        std::shared_ptr<CompositeEffect> effect;

        if (!m_InactiveEffects[type].empty()) {
            effect = m_InactiveEffects[type].front();
            m_InactiveEffects[type].pop();

            // LOG_DEBUG("Retrieved effect from pool, type: {}", static_cast<int>(type));
        } else {
            effect = EffectFactory::GetInstance().CreateEffect(type);
            effect->GetBaseShape()->SetUserData(static_cast<int>(type));

            // LOG_DEBUG("Created new effect, type: {}", static_cast<int>(type));
        }

        m_ActiveEffects.push_back(effect);
        return effect;
    }

    void EffectManager::Draw() {
        for (auto& effect : m_ActiveEffects) {
            if (effect->IsActive()) {
                auto data = Util::ConvertToUniformBufferData(
                    Util::Transform{effect->GetPosition(), 0, {1, 1}},
                    effect->GetSize(),
                    effect->GetBaseShape()->GetZIndex()
                );
                effect->Draw(data);
            }
        }
    }

    std::shared_ptr<CompositeEffect> EffectManager::PlayEffect(
        EffectType type,
        const glm::vec2& position,
        float zIndex,
        float duration
    ) {
        auto effect = GetEffect(type);

        effect->SetDuration(duration);
        effect->Play(position, zIndex);

        return effect;
    }

    void EffectManager::Update(float deltaTime) {
        for (auto it = m_ActiveEffects.begin(); it != m_ActiveEffects.end();) {
            auto effect = *it;
            effect->Update(deltaTime);

            if (effect->IsFinished()) {
                effect->Reset();

                int typeValue = effect->GetBaseShape()->GetUserData();
                EffectType type = static_cast<EffectType>(typeValue);

                m_InactiveEffects[type].push(effect);
                it = m_ActiveEffects.erase(it);
            } else {
                ++it;
            }
        }
    }
}