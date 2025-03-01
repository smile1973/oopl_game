#include "Effect/CircleEffect.hpp"
#include "Util/Logger.hpp"
#include "Util/TransformUtils.hpp"
#include "config.hpp"

namespace Effect {

    std::unique_ptr<Core::Program> CircleEffect::s_Program = nullptr;
    std::unique_ptr<Core::VertexArray> CircleEffect::s_VertexArray = nullptr;

    CircleEffect::CircleEffect(float radius, float duration, const Util::Color& color)
        : m_Radius(radius), m_Color(color) {

        m_Duration = duration * 4.0f;

        if (s_Program == nullptr) {
            InitProgram();
        }

        if (s_VertexArray == nullptr) {
            InitVertexArray();
        }

        m_MatricesBuffer = std::make_unique<Core::UniformBuffer<Core::Matrices>>(
            *s_Program, "Matrices", 0);

        InitUniforms();
    }

    CircleEffect::~CircleEffect() = default;

    void CircleEffect::Draw(const Core::Matrices& data) {
        if (m_State != State::ACTIVE) return;

        m_MatricesBuffer->SetData(0, data);

        // 確保啟用混合
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        s_Program->Bind();

        // 更新著色器參數
        glUniform1f(m_RadiusLocation, m_Radius);
        glUniform1f(m_ThicknessLocation, m_Thickness);
        glUniform1f(m_FadeLocation, m_FadeWidth);
        glUniform4f(m_ColorLocation, m_Color.r, m_Color.g, m_Color.b, m_Color.a);
        glUniform1f(m_TimeLocation, m_ElapsedTime);

        s_Program->Validate();

        s_VertexArray->Bind();
        s_VertexArray->DrawTriangles();
    }

    void CircleEffect::Update(float deltaTime) {
        if (m_State == State::ACTIVE) {
            m_ElapsedTime += deltaTime;
            if (m_ElapsedTime >= m_Duration) {
                m_State = State::FINISHED;
            }
        }
    }

    void CircleEffect::Reset() {
        m_ElapsedTime = 0.0f;
        m_State = State::INACTIVE;
    }

    void CircleEffect::Play(const glm::vec2& position, float zIndex) {
        Reset();
        m_Transform.translation = position;
        m_ZIndex = zIndex;
        m_State = State::ACTIVE;
    }

    void CircleEffect::InitProgram() {
        try {
            s_Program = std::make_unique<Core::Program>(
                GA_RESOURCE_DIR "/shaders/Circle.vert",
                GA_RESOURCE_DIR "/shaders/Circle.frag");
            LOG_INFO("Circle effect shaders loaded successfully");
        } catch (const std::exception& e) {
            LOG_ERROR("Failed to load circle effect shaders: {}", e.what());
        }
    }

    void CircleEffect::InitVertexArray() {
        s_VertexArray = std::make_unique<Core::VertexArray>();

        // 指定一個正方形，將在著色器中轉換為圓形
        s_VertexArray->AddVertexBuffer(std::make_unique<Core::VertexBuffer>(
            std::vector<float>{
                -0.5f, 0.5f,   // 左上
                -0.5f, -0.5f,  // 左下
                0.5f, -0.5f,   // 右下
                0.5f, 0.5f     // 右上
            },
            2));

        // UV 坐標
        s_VertexArray->AddVertexBuffer(std::make_unique<Core::VertexBuffer>(
            std::vector<float>{
                0.0f, 0.0f,  // 左上
                0.0f, 1.0f,  // 左下
                1.0f, 1.0f,  // 右下
                1.0f, 0.0f   // 右上
            },
            2));

        // 頂點索引
        s_VertexArray->SetIndexBuffer(std::make_unique<Core::IndexBuffer>(
            std::vector<unsigned int>{
                0, 1, 2,  // 第一個三角形
                0, 2, 3   // 第二個三角形
            }));
    }

    void CircleEffect::InitUniforms() {
        s_Program->Bind();

        m_RadiusLocation = glGetUniformLocation(s_Program->GetId(), "u_Radius");
        m_ThicknessLocation = glGetUniformLocation(s_Program->GetId(), "u_Thickness");
        m_FadeLocation = glGetUniformLocation(s_Program->GetId(), "u_Fade");
        m_ColorLocation = glGetUniformLocation(s_Program->GetId(), "u_Color");
        m_TimeLocation = glGetUniformLocation(s_Program->GetId(), "u_Time");

        if (m_RadiusLocation == -1 || m_ThicknessLocation == -1 ||
            m_FadeLocation == -1 || m_ColorLocation == -1 || m_TimeLocation == -1) {
            LOG_ERROR("Failed to get uniform locations for CircleEffect");
        }
    }
}