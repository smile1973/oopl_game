#include "Effect/Shape/RectangleShape.hpp"
#include "Util/Logger.hpp"
#include "config.hpp"

namespace Effect {
    namespace Shape {

        std::unique_ptr<Core::Program> RectangleShape::s_Program = nullptr;
        std::unique_ptr<Core::VertexArray> RectangleShape::s_VertexArray = nullptr;

        RectangleShape::RectangleShape(const glm::vec2& dimensions, float thickness, float rotation, float duration, bool autoRotate, float rotationSpeed)
            : BaseShape(duration), m_Dimensions(dimensions), m_Thickness(thickness), m_Rotation(rotation),
              m_AutoRotate(autoRotate), m_RotationSpeed(rotationSpeed) {

            // Initialize OpenGL resources
            if (s_Program == nullptr || s_VertexArray == nullptr) {
                RectangleShape::InitializeResources();
            }

            m_MatricesBuffer = std::make_unique<Core::UniformBuffer<Core::Matrices>>(
                *s_Program, "Matrices", 0);

            // Get uniform locations for this instance
            s_Program->Bind();
            m_DimensionsLocation = glGetUniformLocation(s_Program->GetId(), "u_Dimensions");
            m_ThicknessLocation = glGetUniformLocation(s_Program->GetId(), "u_Thickness");
            m_RotationLocation = glGetUniformLocation(s_Program->GetId(), "u_Rotation");
            m_ColorLocation = glGetUniformLocation(s_Program->GetId(), "u_Color");
            m_TimeLocation = glGetUniformLocation(s_Program->GetId(), "u_Time");

            if (m_DimensionsLocation == -1 || m_ThicknessLocation == -1 ||
                m_RotationLocation == -1 || m_ColorLocation == -1 ||
                m_TimeLocation == -1) {
                LOG_ERROR("Failed to get basic uniform locations for RectangleShape instance");
            }

            GLint fillTypeLocation = glGetUniformLocation(s_Program->GetId(), "u_FillType");
            GLint fillThicknessLocation = glGetUniformLocation(s_Program->GetId(), "u_FillThickness");
            GLint edgeTypeLocation = glGetUniformLocation(s_Program->GetId(), "u_EdgeType");
            GLint edgeWidthLocation = glGetUniformLocation(s_Program->GetId(), "u_EdgeWidth");
            GLint edgeColorLocation = glGetUniformLocation(s_Program->GetId(), "u_EdgeColor");
            GLint animTypeLocation = glGetUniformLocation(s_Program->GetId(), "u_AnimType");
            GLint intensityLocation = glGetUniformLocation(s_Program->GetId(), "u_Intensity");
            GLint speedLocation = glGetUniformLocation(s_Program->GetId(), "u_AnimSpeed");

            if (fillTypeLocation == -1 || fillThicknessLocation == -1) {
                LOG_ERROR("Failed to get uniform locations for FillModifier in RectangleShape");
            }
            if (edgeTypeLocation == -1 || edgeWidthLocation == -1 || edgeColorLocation == -1) {
                LOG_ERROR("Failed to get uniform locations for EdgeModifier in RectangleShape");
            }
            if (animTypeLocation == -1 || intensityLocation == -1 || speedLocation == -1) {
                LOG_ERROR("Failed to get uniform locations for AnimationModifier in RectangleShape");
            }
        }

        RectangleShape::~RectangleShape() = default;

        void RectangleShape::Draw(const Core::Matrices& data) {
            if (m_State != State::ACTIVE) return;

            // Update matrices
            m_MatricesBuffer->SetData(0, data);

            // Enable blending
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // Bind shader program
            s_Program->Bind();

            // Set uniforms with instance values
            glUniform2f(m_DimensionsLocation, m_Dimensions.x, m_Dimensions.y);
            glUniform1f(m_ThicknessLocation, m_Thickness); // 保持厚度的設置
            glUniform1f(m_RotationLocation, m_Rotation);

            // Set color properly
            glUniform4f(m_ColorLocation, m_Color.r, m_Color.g, m_Color.b, m_Color.a);

            // Set time for animation
            glUniform1f(m_TimeLocation, m_ElapsedTime);

            // Validate shader program
            s_Program->Validate();

            // Draw
            s_VertexArray->Bind();
            s_VertexArray->DrawTriangles();
        }
        void RectangleShape::Update(float deltaTime) {
            BaseShape::Update(deltaTime);

            if (m_AutoRotate && m_State == State::ACTIVE) {
                m_Rotation += deltaTime * m_RotationSpeed * 3.14159f;
                // 保持旋轉角度在0-2π範圍內
                while (m_Rotation > 2.0f * 3.14159f) {
                    m_Rotation -= 2.0f * 3.14159f;
                }
            }
        }

        void RectangleShape::InitializeResources() {
            // Initialize shader program
            try {
                s_Program = std::make_unique<Core::Program>(
                    GA_RESOURCE_DIR "/shaders/Rectangle.vert",
                    GA_RESOURCE_DIR "/shaders/Rectangle.frag");
                LOG_INFO("Rectangle shape shaders loaded successfully");
            } catch (const std::exception& e) {
                LOG_ERROR("Failed to load rectangle shape shaders: {}", e.what());
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