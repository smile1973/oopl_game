#ifndef PROJECTILE_EFFECT_HPP
#define PROJECTILE_EFFECT_HPP

#include "Effect/Effect.hpp"
#include "Core/Program.hpp"
#include "Core/VertexArray.hpp"
#include "Core/UniformBuffer.hpp"
#include "Util/Color.hpp"

namespace Effect {
    class ProjectileEffect : public IEffect {
    public:
        ProjectileEffect(float radius = 0.4f, float duration = 0.75f, float distance = 200.0f,
                        const Util::Color& color = Util::Color::FromName(Util::Colors::PINK));
        ~ProjectileEffect() override;

        // 實現自 Core::Drawable
        void Draw(const Core::Matrices& data) override;
        glm::vec2 GetSize() const override { return m_Size; }

        // 實現自 Effect::IEffect
        void Update(float deltaTime) override;
        void Play(const glm::vec2& position, float zIndex = 0.0f) override;
        void Reset() override;

        // 設置參數
        void SetRadius(float radius) { m_Radius = radius; }
        void SetColor(const Util::Color& color) { m_Color = color; }
        void SetDistance(float distance) { m_Distance = distance; }
        void SetDirection(const glm::vec2& direction) { m_Direction = glm::normalize(direction); }
        void SetSpeed(float speed) { m_Speed = speed; }
        void SetSize(const glm::vec2& size) { m_Size = size; }

    private:
        void InitProgram();
        void InitVertexArray();
        void InitUniforms();

        static std::unique_ptr<Core::Program> s_Program;
        static std::unique_ptr<Core::VertexArray> s_VertexArray;

        std::unique_ptr<Core::UniformBuffer<Core::Matrices>> m_MatricesBuffer;

        GLint m_RadiusLocation = -1;
        GLint m_ColorLocation = -1;
        GLint m_TimeLocation = -1;
        
        float m_Radius = 0.4f;            // 圓的半徑
        float m_Distance = 200.0f;        // 投射物飛行距離
        float m_Speed = 300.0f;           // 投射物速度
        glm::vec2 m_Direction = {1.0f, 0.0f}; // 投射物方向
        glm::vec2 m_StartPosition = {0.0f, 0.0f}; // 起始位置
        Util::Color m_Color;              // 圓的顏色
        glm::vec2 m_Size = {100, 100};    // 效果的大小
        float m_DistanceTraveled = 0.0f;  // 已飛行距離
    };
}

#endif // PROJECTILE_EFFECT_HPP