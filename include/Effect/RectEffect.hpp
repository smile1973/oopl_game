#ifndef RECT_EFFECT_HPP
#define RECT_EFFECT_HPP

#include "Effect.hpp"
#include "Core/Program.hpp"
#include "Core/VertexArray.hpp"
#include "Core/UniformBuffer.hpp"
#include "Util/Color.hpp"

namespace Effect {
    class RectEffect : public IEffect {
    public:
        RectEffect(const glm::vec2& size = {0.4f, 0.4f}, float duration = 0.75f,
                  const Util::Color& color = Util::Color::FromName(Util::Colors::WHITE));
        ~RectEffect() override;

        // 實現自 Core::Drawable
        void Draw(const Core::Matrices& data) override;
        glm::vec2 GetSize() const override { return m_Dimensions; }

        // 實現自 Effect::IEffect
        void Update(float deltaTime) override;
        void Play(const glm::vec2& position, float zIndex = 0.0f) override;
        void Reset() override;

        // 設置參數
        void SetDimensions(const glm::vec2& dimensions) { m_Dimensions = dimensions; }
        void SetColor(const Util::Color& color) { m_Color = color; }
        void SetBorderThickness(float thickness) { m_BorderThickness = thickness; }
        void SetFade(float fade) { m_FadeWidth = fade; }

    private:
        void InitProgram();
        void InitVertexArray();
        void InitUniforms();

        static std::unique_ptr<Core::Program> s_Program;
        static std::unique_ptr<Core::VertexArray> s_VertexArray;

        std::unique_ptr<Core::UniformBuffer<Core::Matrices>> m_MatricesBuffer;

        GLint m_DimensionsLocation = -1;
        GLint m_BorderThicknessLocation = -1;
        GLint m_FadeLocation = -1;
        GLint m_ColorLocation = -1;
        GLint m_TimeLocation = -1;

        glm::vec2 m_Dimensions = {0.4f, 0.4f};  // 矩形的尺寸
        float m_BorderThickness = 0.02f;        // 邊框寬度
        float m_FadeWidth = 0.05f;              // 漸變效果的寬度
        Util::Color m_Color;                    // 矩形的顏色
    };
}

#endif // RECT_EFFECT_HPP