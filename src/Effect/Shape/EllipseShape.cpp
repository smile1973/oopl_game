#include "Effect/Shape/EllipseShape.hpp"
#include "Util/Logger.hpp"
#include "config.hpp"

namespace Effect {
    namespace Shape {

        std::unique_ptr<Core::Program> EllipseShape::s_Program = nullptr;
        std::unique_ptr<Core::VertexArray> EllipseShape::s_VertexArray = nullptr;

        EllipseShape::EllipseShape(const glm::vec2& radii, float duration)
            : BaseShape(duration), m_Radii(radii) {

            // 初始化OpenGL資源
            if (s_Program == nullptr || s_VertexArray == nullptr) {
                EllipseShape::InitializeResources();
            }

            m_MatricesBuffer = std::make_unique<Core::UniformBuffer<Core::Matrices>>(
                *s_Program, "Matrices", 0);
        }

        EllipseShape::~EllipseShape() = default;

        void EllipseShape::Draw(const Core::Matrices& data) {
            if (m_State != State::ACTIVE) return;

            // 更新矩陣
            m_MatricesBuffer->SetData(0, data);

            // 啟用混合
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // 綁定著色器程序
            s_Program->Bind();

            // 設置uniforms
            glUniform2f(m_RadiiLocation, m_Radii.x, m_Radii.y);
            glUniform4f(m_ColorLocation, m_Color.r, m_Color.g, m_Color.b, m_Color.a);
            glUniform1f(m_TimeLocation, m_ElapsedTime);

            // 驗證著色器程序
            s_Program->Validate();

            // 繪製
            s_VertexArray->Bind();
            s_VertexArray->DrawTriangles();
        }

        void EllipseShape::InitializeResources() {
            // 初始化著色器程序
            try {
                s_Program = std::make_unique<Core::Program>(
                    GA_RESOURCE_DIR "/shaders/Ellipse.vert",
                    GA_RESOURCE_DIR "/shaders/Ellipse.frag");
                LOG_INFO("Ellipse shape shaders loaded successfully");
            } catch (const std::exception& e) {
                LOG_ERROR("Failed to load ellipse shape shaders: {}", e.what());
            }

            // 獲取uniform位置
            s_Program->Bind();
            m_RadiiLocation = glGetUniformLocation(s_Program->GetId(), "u_Radii");
            m_ColorLocation = glGetUniformLocation(s_Program->GetId(), "u_Color");
            m_TimeLocation = glGetUniformLocation(s_Program->GetId(), "u_Time");

            if (m_RadiiLocation == -1 || m_ColorLocation == -1 || m_TimeLocation == -1) {
                LOG_ERROR("Failed to get uniform locations for EllipseShape");
            }

            // 初始化頂點數組
            s_VertexArray = std::make_unique<Core::VertexArray>();

            // 設置頂點數據
            s_VertexArray->AddVertexBuffer(std::make_unique<Core::VertexBuffer>(
                std::vector<float>{
                    -0.5f, 0.5f,   // 左上
                    -0.5f, -0.5f,  // 左下
                    0.5f, -0.5f,   // 右下
                    0.5f, 0.5f     // 右上
                },
                2));

            // 設置UV坐標
            s_VertexArray->AddVertexBuffer(std::make_unique<Core::VertexBuffer>(
                std::vector<float>{
                    0.0f, 0.0f,  // 左上
                    0.0f, 1.0f,  // 左下
                    1.0f, 1.0f,  // 右下
                    1.0f, 0.0f   // 右上
                },
                2));

            // 設置索引
            s_VertexArray->SetIndexBuffer(std::make_unique<Core::IndexBuffer>(
                std::vector<unsigned int>{
                    0, 1, 2,  // 第一個三角形
                    0, 2, 3   // 第二個三角形
                }));
        }

    } // namespace Shape
} // namespace Effect