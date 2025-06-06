#ifndef RECTANGLESHAPE_HPP
#define RECTANGLESHAPE_HPP

#include "BaseShape.hpp"
#include "config.hpp"

namespace Effect {
    namespace Shape {
        class RectangleShape : public BaseShape {
        public:
            RectangleShape(const glm::vec2& dimensions = {0.8f, 0.1f},
                        float thickness = 0.03f,
                        float rotation = 0.0f,
                        float duration = 1.0f,
                        bool autoRotate = false,
                        float rotationSpeed = 2.0f);
            ~RectangleShape() override;

            void Draw(const Core::Matrices& data) override;
            glm::vec2 GetSize() const override { return m_Size; }

            void SetAutoRotation(bool enable, float speed = 2.0f) {
                m_AutoRotate = enable;
                m_RotationSpeed = speed;
            }
            bool IsAutoRotating() const { return m_AutoRotate; }
            float GetRotationSpeed() const { return m_RotationSpeed; }

            void Update(float deltaTime) override;

            void SetDimensions(const glm::vec2& dimensions) { m_Dimensions = dimensions; }
            const glm::vec2& GetDimensions() const { return m_Dimensions; }

            void SetThickness(float thickness) { m_Thickness = thickness; }
            float GetThickness() const { return m_Thickness; }

            void SetRotation(float rotation) { m_Rotation = rotation; }
            float GetRotation() const { return m_Rotation; }

            void SetSize(const glm::vec2& size) { m_Size = size; }

            static Core::Program* GetProgram() { return s_Program.get(); }
            static Core::VertexArray* GetVertexArray() { return s_VertexArray.get(); }

        protected:
            void InitializeResources() override;

            static std::unique_ptr<Core::Program> s_Program;
            static std::unique_ptr<Core::VertexArray> s_VertexArray;

            std::unique_ptr<Core::UniformBuffer<Core::Matrices>> m_MatricesBuffer;

            GLint m_DimensionsLocation = -1;
            GLint m_ThicknessLocation = -1;
            GLint m_RotationLocation = -1;
            GLint m_ColorLocation = -1;
            GLint m_TimeLocation = -1;

            glm::vec2 m_Dimensions = {0.8f, 0.1f};
            float m_Thickness = 0.03f;
            float m_Rotation = 0.0f;
            glm::vec2 m_Size = {400, 400};

            bool m_AutoRotate = false;
            float m_RotationSpeed = 2.0f;          // 旋轉速度 (弧度/秒)
        };

    }
}

#endif