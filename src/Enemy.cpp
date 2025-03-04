#include "Enemy.hpp"

std::unique_ptr<Core::Program> Enemy::s_Program = nullptr;
std::unique_ptr<Core::VertexArray> Enemy::s_VertexArray = nullptr;

Enemy::Enemy(float health, const std::vector<std::string>& ImageSet)
    : Character(ImageSet), m_Health(health), m_MaxHealth(health),
      m_Width(100.0f), m_Height(10.0f) {

    if (!s_Program) {
        InitProgram();
    }
    if (!s_VertexArray) {
        InitVertexArray();
    }

    InitUniforms();
}

void Enemy::TakeDamage(float damage) {
    m_Health = std::max(0.0f, m_Health - damage);
    LOG_DEBUG("Enemy took {:.1f} damage, remaining health: {:.1f}", damage, m_Health);
}

bool Enemy::IsAlive() const {
    return m_Health > 0.0f;
}

void Enemy::DrawHealthBar(const glm::vec2& position) {
    if (!s_Program || !s_VertexArray) return;

    // 設定血條位置
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    s_Program->Bind();

    // 設定固定紅色
    glUniform4f(m_ColorLocation, 1.0f, 0.0f, 0.0f, 1.0f);

    // 依據當前血量計算血條長度，確保從左向右縮短
    float currentWidth = (m_Health / m_MaxHealth) * m_Width;
    glUniform1f(m_WidthLocation, currentWidth);

    s_Program->Validate();

    s_VertexArray->Bind();
    s_VertexArray->DrawTriangles();
}

void Enemy::InitProgram() {
    try {
        s_Program = std::make_unique<Core::Program>(
            GA_RESOURCE_DIR "/Shader/HealthBar.vert",
            GA_RESOURCE_DIR "/Shader/HealthBar.frag"
        );
        LOG_INFO("Health bar shaders loaded successfully");
    } catch (const std::exception& e) {
        LOG_ERROR("Failed to load health bar shaders: {}", e.what());
        s_Program.reset();
    }
}

void Enemy::InitVertexArray() {
    s_VertexArray = std::make_unique<Core::VertexArray>();

    // 血條矩形的四個頂點（確保左對齊縮放）
    float halfWidth = 0.5f, halfHeight = 0.5f;
    s_VertexArray->AddVertexBuffer(std::make_unique<Core::VertexBuffer>(std::vector<float>{
        -halfWidth,  halfHeight,  // 左上
        -halfWidth, -halfHeight,  // 左下
         halfWidth, -halfHeight,  // 右下
         halfWidth,  halfHeight   // 右上
    }, 2));

    // UV 坐標
    s_VertexArray->AddVertexBuffer(std::make_unique<Core::VertexBuffer>(std::vector<float>{
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    }, 2));

    // 頂點索引
    s_VertexArray->SetIndexBuffer(std::make_unique<Core::IndexBuffer>(std::vector<unsigned int>{
        0, 1, 2,  // 第一個三角形
        0, 2, 3   // 第二個三角形
    }));
}

void Enemy::InitUniforms() {
    if (!s_Program) return;

    s_Program->Bind();
    m_ColorLocation = glGetUniformLocation(s_Program->GetId(), "u_Color");
    m_WidthLocation = glGetUniformLocation(s_Program->GetId(), "u_Width");

    if (m_ColorLocation == -1 || m_WidthLocation == -1) {
        LOG_ERROR("Failed to get uniform locations for HealthBar");
    }
}