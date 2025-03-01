#ifndef EFFECT_FACTORY_HPP
#define EFFECT_FACTORY_HPP

#include "Effect.hpp"
#include "CircleEffect.hpp"
#include "RectEffect.hpp"
#include <memory>
#include <unordered_map>
#include <functional>

namespace Effect {
    enum class EffectType {
        CIRCLE,
        RECT,
        // 未來可以添加更多特效類型
    };

    class EffectFactory {
    public:
        static EffectFactory& GetInstance() {
            static EffectFactory instance;
            return instance;
        }

        // 根據類型創建特效
        std::shared_ptr<IEffect> CreateEffect(EffectType type) {
            auto it = m_Creators.find(type);
            if (it != m_Creators.end()) {
                return it->second();
            }
            return nullptr;
        }

    private:
        EffectFactory() {
            // 註冊特效創建函數
            m_Creators[EffectType::CIRCLE] = []() { return std::make_shared<CircleEffect>(); };
            // m_Creators[EffectType::RECT] = []() { return std::make_shared<RectEffect>(); };
        }

        std::unordered_map<EffectType, std::function<std::shared_ptr<IEffect>()>> m_Creators;
    };
}

#endif // EFFECT_FACTORY_HPP