#include "Effect/EffectManager.hpp"
#include "Util/TransformUtils.hpp"
#include "Util/Logger.hpp"

namespace Effect {

    void EffectManager::Initialize(size_t initialPoolSize) {
        EffectType initOrder[] = {
            EffectType::SKILL_Z,  // 可能包含基本的著色器初始化
            EffectType::SKILL_X,
            EffectType::SKILL_C,
            EffectType::SKILL_V
        };
        for (auto type : initOrder) {
            for (size_t j = 0; j < initialPoolSize; ++j) {
                auto effect = EffectFactory::GetInstance().CreateEffect(type);
                m_InactiveEffects[type].push(effect);
            }
        }
        LOG_INFO("EffectManager initialized effect");
    }

    std::shared_ptr<CompositeEffect> EffectManager::GetEffect(EffectType type) {
        // LOG_DEBUG("request effect type: {}, size: {}",
        //      static_cast<int>(type),
        //      m_InactiveEffects[type].size());
        std::shared_ptr<CompositeEffect> effect;

        // 嘗試從對象池獲取特效
        if (!m_InactiveEffects[type].empty()) {
            effect = m_InactiveEffects[type].front();
            m_InactiveEffects[type].pop();

            LOG_DEBUG("Retrieved effect from pool, type: {}", static_cast<int>(type));
        } else {
            // 創建新的特效
            effect = EffectFactory::GetInstance().CreateEffect(type);
            LOG_DEBUG("Created new effect, type: {}", static_cast<int>(type));
        }

        // 添加到活躍特效列表
        m_ActiveEffects.push_back(effect);
        return effect;
    }

    void EffectManager::Draw() {
        // 這個方法會被GameObject的Draw方法調用
        for (auto& effect : m_ActiveEffects) {
            if (effect->IsActive()) {
                // 獲取變換矩陣
                auto data = Util::ConvertToUniformBufferData(
                    Util::Transform{effect->GetPosition(), 0, {1, 1}},
                    effect->GetSize(),
                    m_ZIndex
                );

                // 繪製特效
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
        // 獲取特效
        auto effect = GetEffect(type);
        
        // 設置持續時間
        effect->SetDuration(duration);
        
        // 播放特效
        effect->Play(position, zIndex);
        
        return effect;
    }
    
    void EffectManager::Update(float deltaTime) {
        // 更新所有活躍特效
        for (auto it = m_ActiveEffects.begin(); it != m_ActiveEffects.end();) {
            auto effect = *it;
            effect->Update(deltaTime);
            
            if (effect->IsFinished()) {
                // 重置並回收特效
                effect->Reset();
                
                // 將特效放回對象池
                // 簡化這裡的實現，只將特效放回第一個類型的隊列
                // 在實際應用中，您可能想要跟踪每個特效的具體類型
                if (!m_InactiveEffects.empty()) {
                    m_InactiveEffects.begin()->second.push(effect);
                }
                
                // 從活躍列表中移除
                it = m_ActiveEffects.erase(it);
            } else {
                ++it;
            }
        }
    }
}