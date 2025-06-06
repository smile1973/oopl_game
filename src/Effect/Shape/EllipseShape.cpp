#include "Effect/Shape/EllipseShape.hpp"
#include "Util/Logger.hpp"
#include "config.hpp"

namespace Effect {
    namespace Shape {

        std::unique_ptr<Core::Program> EllipseShape::s_Program = nullptr;
        std::unique_ptr<Core::VertexArray> EllipseShape::s_VertexArray = nullptr;

        EllipseShape::EllipseShape(const glm::vec2& radii, float duration)
            : BaseShape(duration), m_Radii(radii) {

            if (s_Program == nullptr || s_VertexArray == nullptr) {
                EllipseShape::InitializeResources();
            }

            m_MatricesBuffer = std::make_unique<Core::UniformBuffer<Core::Matrices>>(
                *s_Program, "Matrices", 0);

            s_Program->Bind();
            m_RadiiLocation = glGetUniformLocation(s_Program->GetId(), "u_Radii");
            m_ColorLocation = glGetUniformLocation(s_Program->GetId(), "u_Color");
            m_TimeLocation = glGetUniformLocation(s_Program->GetId(), "u_Time");

            if (m_RadiiLocation == -1 || m_ColorLocation == -1 || m_TimeLocation == -1) {
                LOG_ERROR("Failed to get uniform locations for EllipseShape instance");
            }
        }

        EllipseShape::~EllipseShape() = default;

        void EllipseShape::Draw(const Core::Matrices& data) {
            if (m_State != State::ACTIVE) return;

            m_MatricesBuffer->SetData(0, data);

            // Enable blending
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glUniform2f(m_RadiiLocation, m_Radii.x, m_Radii.y);
            // 設置顏色
            glUniform4f(m_ColorLocation, m_Color.r, m_Color.g, m_Color.b, m_Color.a);
            // 設置時間
            glUniform1f(m_TimeLocation, m_ElapsedTime);

            // Draw
            s_VertexArray->Bind();
            s_VertexArray->DrawTriangles();
        }

        void EllipseShape::InitializeResources() {
            try {
                s_Program = std::make_unique<Core::Program>(
                    GA_RESOURCE_DIR "/shaders/Ellipse.vert",
                    GA_RESOURCE_DIR "/shaders/Ellipse.frag");
                LOG_INFO("Ellipse shape shaders loaded successfully");
            } catch (const std::exception& e) {
                LOG_ERROR("Failed to load ellipse shape shaders: {}", e.what());
            }

            // Initialize vertex array
            s_VertexArray = std::make_unique<Core::VertexArray>();

            // Set vertex data
            s_VertexArray->AddVertexBuffer(std::make_unique<Core::VertexBuffer>(
                std::vector<float>{
                    -0.5f, 0.5f,   // top left
                    -0.5f, -0.5f,  // bottom left
                    0.5f, -0.5f,   // bottom right
                    0.5f, 0.5f     // top right
                },
                2));

            // Set UV coordinates
            s_VertexArray->AddVertexBuffer(std::make_unique<Core::VertexBuffer>(
                std::vector<float>{
                    0.0f, 0.0f,  // top left
                    0.0f, 1.0f,  // bottom left
                    1.0f, 1.0f,  // bottom right
                    1.0f, 0.0f   // top right
                },
                2));

            // Set indices
            s_VertexArray->SetIndexBuffer(std::make_unique<Core::IndexBuffer>(
                std::vector<unsigned int>{
                    0, 1, 2,  // first triangle
                    0, 2, 3   // second triangle
                }));
        }

    }
}