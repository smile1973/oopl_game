// include/Effect/EffectFactory.hpp - 最終優化版本
#ifndef EFFECT_FACTORY_HPP
#define EFFECT_FACTORY_HPP

#include "Effect/CompositeEffect.hpp"

namespace Effect {

    // 預定義特效類型
    enum class EffectType {
        // 角色技能特效
        SKILL_Z,        // Z技能 : 實心圓，邊緣發光
        SKILL_X,        // X技能 : 空心圓，邊緣發光，會移動
        SKILL_C,        // C技能 : 實心橢圓，邊緣發光
        SKILL_V,        // V技能 : 實心圓，強發光邊緣

        ENEMY_ATTACK_2, // 空心圓，橙色發光邊緣

        RECT_LASER,     // 矩形雷射，實心，邊緣發光
        RECT_BEAM,      // 矩形光束，可旋轉，邊緣發光
    };

    class EffectFactory {
    public:
        static EffectFactory& GetInstance() {
            static EffectFactory instance;
            return instance;
        }

        // 創建預定義特效
        static std::shared_ptr<CompositeEffect> CreateEffect(EffectType type);

        // 創建自定義組合特效（移除動畫參數）
        std::shared_ptr<CompositeEffect> CreateCustomEffect(
            bool isCircle = true,
            const Modifier::FillType& fillType = Modifier::FillType::SOLID,
            const Modifier::EdgeType& edgeType = Modifier::EdgeType::NONE,
            bool isMoving = false
        );

    private:
        EffectFactory() = default;

        // 私有輔助方法：減少重複代碼
        static std::shared_ptr<CompositeEffect> CreateCircleEffect(
            float radius,
            const Util::Color& color,
            float duration,
            Modifier::FillType fillType = Modifier::FillType::SOLID,
            Modifier::EdgeType edgeType = Modifier::EdgeType::NONE,
            const glm::vec2& size = {400, 400},
            float edgeWidth = 0.05f
        );

        static std::shared_ptr<CompositeEffect> CreateRectangleEffect(
            const glm::vec2& dimensions,
            const Util::Color& color,
            float duration,
            bool autoRotate = false,
            float rotationSpeed = 0.0f,
            const glm::vec2& size = {1600, 1600}
        );
    };

} // namespace Effect

#endif // EFFECT_FACTORY_HPP