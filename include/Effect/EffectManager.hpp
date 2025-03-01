#ifndef EFFECT_MANAGER_HPP
#define EFFECT_MANAGER_HPP

#include <queue>

#include "Effect.hpp"
#include "EffectFactory.hpp"
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

        // 初始化對象池
        void Initialize(size_t initialCircleEffects = 4) {
            // 預創建一些特效對象
            for (size_t i = 0; i < initialCircleEffects; i++) {
                auto effect = std::make_shared<CircleEffect>();
                m_InactiveEffects[EffectType::CIRCLE].push(effect);
            }

            // for (size_t i = 0; i < initialRectEffects; i++) {
            //     auto effect = std::make_shared<RectEffect>();
            //     m_InactiveEffects[EffectType::RECT].push(effect);
            // }
        }

        // 獲取或創建特效
        std::shared_ptr<IEffect> GetEffect(EffectType type) {
            std::shared_ptr<IEffect> effect;

            // 檢查是否有可用的特效對象
            if (!m_InactiveEffects[type].empty()) {
                effect = m_InactiveEffects[type].front();
                m_InactiveEffects[type].pop();
            } else {
                // 如果沒有可用對象，創建新的
                effect = EffectFactory::GetInstance().CreateEffect(type);
            }

            // 添加到活躍特效列表
            m_ActiveEffects.push_back(effect);
            return effect;
        }

        // 更新所有特效
        void Update(float deltaTime) {
            // 更新所有活躍特效
            for (auto it = m_ActiveEffects.begin(); it != m_ActiveEffects.end();) {
                auto effect = *it;
                effect->Update(deltaTime);

                if (effect->IsFinished()) {
                    // 重置並回收特效
                    effect->Reset();

                    // 將特效放回對象池中對應的類型隊列
                    if (auto circleEffect = std::dynamic_pointer_cast<CircleEffect>(effect)) {
                        m_InactiveEffects[EffectType::CIRCLE].push(effect);
                    } else if (auto rectEffect = std::dynamic_pointer_cast<RectEffect>(effect)) {
                        m_InactiveEffects[EffectType::RECT].push(effect);
                    }

                    // 從活躍列表中移除
                    it = m_ActiveEffects.erase(it);
                } else {
                    ++it;
                }
            }
        }

        // 繪製所有特效
        void Draw() override{
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

        // 創建並播放特效
        template<typename EffectType, typename... Args>
        std::shared_ptr<EffectType> PlayEffect(const glm::vec2& position, float zIndex, Args&&... args) {
            // 創建特效
            auto effect = std::make_shared<EffectType>(std::forward<Args>(args)...);

            // 設置位置和播放
            effect->Play(position, zIndex);

            // 添加到活躍列表
            m_ActiveEffects.push_back(effect);

            return effect;
        }

    private:
        EffectManager() : Util::GameObject(nullptr, 100) {} // 設置高的z-index確保特效在最上層

        // 特效對象池，按類型分類
        std::unordered_map<EffectType, std::queue<std::shared_ptr<IEffect>>> m_InactiveEffects;

        // 當前活躍的特效
        std::vector<std::shared_ptr<IEffect>> m_ActiveEffects;
    };
}

#endif // EFFECT_MANAGER_HPP