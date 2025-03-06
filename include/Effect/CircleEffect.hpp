#ifndef CIRCLE_EFFECT_HPP
#define CIRCLE_EFFECT_HPP

#include "Effect.hpp"
#include "Core/Program.hpp"
#include "Core/VertexArray.hpp"
#include "Core/UniformBuffer.hpp"
#include "Util/Color.hpp"

namespace Effect {
    class CircleEffect : public IEffect {
    public:
        CircleEffect(float radius = 0.4f, float duration = 0.75f,
                    const Util::Color& color = Util::Color::FromName(Util::Colors::WHITE));
        ~CircleEffect() override;

        // 實現自 Core::Drawable
        void Draw(const Core::Matrices& data) override;
        [[nodiscard]] glm::vec2 GetSize() const override { return m_Size; }

        // 實現自 Effect::IEffect
        void Update(float deltaTime) override;
        void Play(const glm::vec2& position, float zIndex = 0.0f) override;
        void Reset() override;

        // 設置參數
        void SetRadius(float radius) { m_Radius = radius; }
        void SetColor(const Util::Color& color) { m_Color = color; }
        void SetThickness(float thickness) { m_Thickness = thickness; }
        void SetFade(float fade) { m_FadeWidth = fade; }
        void SetSize(const glm::vec2& size) { m_Size = size; }

    private:
        void InitProgram();
        void InitVertexArray();
        void InitUniforms();

        static std::unique_ptr<Core::Program> s_Program;
        static std::unique_ptr<Core::VertexArray> s_VertexArray;

        std::unique_ptr<Core::UniformBuffer<Core::Matrices>> m_MatricesBuffer;

        GLint m_RadiusLocation = -1;
        GLint m_ThicknessLocation = -1;
        GLint m_FadeLocation = -1;
        GLint m_ColorLocation = -1;
        GLint m_TimeLocation = -1;

        float m_Radius = 0.4f;           // 圓的半徑
        float m_Thickness = 0.02f;       // 圓的邊緣寬度
        float m_FadeWidth = 0.05f;       // 漸變效果的寬度
        Util::Color m_Color;             // 圓的顏色
        glm::vec2 m_Size = {400, 400};   // 效果的大小（可自定義）
    };
}

#endif // CIRCLE_EFFECT_HPP