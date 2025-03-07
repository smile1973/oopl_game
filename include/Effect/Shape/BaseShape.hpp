#ifndef BASESHAPE_HPP
#define BASESHAPE_HPP

#include "Effect/IEffect.hpp"
#include "Core/Program.hpp"
#include "Core/VertexArray.hpp"
#include "Core/UniformBuffer.hpp"
#include "Util/Color.hpp"

namespace Effect {
    namespace Shape {

        class BaseShape : public IEffect {
        public:
            BaseShape(float duration = 1.0f);
            virtual ~BaseShape() = default;

            // 從IEffect繼承的方法
            void Draw(const Core::Matrices& data) override = 0;
            void Update(float deltaTime) override;
            void Play(const glm::vec2& position, float zIndex = 0.0f) override;
            void Reset() override;

            // 設置基本屬性
            void SetColor(const Util::Color& color) { m_Color = color; }
            const Util::Color& GetColor() const { return m_Color; }

        protected:
            Util::Color m_Color = Util::Color::FromName(Util::Colors::PINK);
            virtual void InitializeResources() = 0; // 子類必須實現此方法初始化OpenGL資源
        };
    } // namespace Shape
} // namespace Effect

#endif //BASESHAPE_HPP
