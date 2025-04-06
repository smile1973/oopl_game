#include "Enemy.hpp"

// 初始化靜態成員：著色程序和頂點數據
std::unique_ptr<Core::Program> Enemy::s_Program = nullptr;
std::unique_ptr<Core::VertexArray> Enemy::s_VertexArray = nullptr;

// 構造函數，初始化敵人的生命值與繪製屬性
Enemy::Enemy(std::string name, const float health, const std::vector<std::string>& ImageSet)
    : Character(ImageSet), m_Name(std::move(name)), m_Health(health), m_MaxHealth(health),
      m_ColorLocation(-1), m_WidthLocation(-1) {

    m_Transform.scale = {0.5f, 0.5f};
    SetZIndex(10);
    SetVisible(false);

    // 確保著色程序（Shader Program）只初始化一次
    if (!s_Program) {
        InitProgram();
    }
    // 確保頂點數據（Vertex Array）只初始化一次
    if (!s_VertexArray) {
        InitVertexArray();
    }
    // 初始化 Uniform 變數（著色器中的全域變數）
    InitUniforms();
}

// 讓敵人受到傷害，減少生命值
void Enemy::TakeDamage(float damage) {
    if (! this->GetVisibility()) return;
    m_Health = std::max(0.0f, m_Health - damage);
    LOG_DEBUG("{} took {:.1f} damage, remaining health: {:.1f}", m_Name, damage, m_Health);

    if (! this->IfAlive()) {
        this->SetVisible(false);
        LOG_DEBUG("The Enemy {} dies", m_Name);
        if (! this->IfAlive()) {
            this->SetVisible(false);
            Effect::EffectManager::GetInstance().ClearAllEffects();
            LOG_DEBUG("The Enemy dies");
        }
    }
}

void Enemy::SetHealth(const float Health) {
    m_Health = (Health == -1.0f) ? m_MaxHealth : Health;
    m_MaxHealth = (Health == -1.0f) ? m_MaxHealth : Health;
}

// 繪製敵人的血條
void Enemy::DrawHealthBar(const glm::vec2& position) const {
    if (!s_Program || !s_VertexArray) return;

    // 啟用透明度混合，以確保血條能夠正確顯示
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    s_Program->Bind();

    // 設定血條顏色為紅色
    Util::Color m_Color = Util::Color(1.0, 0.1, 0.1, 0.4);
    glUniform4f(m_ColorLocation, m_Color.r, m_Color.g, m_Color.b, m_Color.a);

    // 根據當前生命值調整血條寬度
    float currentWidth = m_Health / m_MaxHealth; // 0 ~ 1 縮放
    glUniform1f(glGetUniformLocation(s_Program->GetId(), "u_Width"), currentWidth);
    glUniform2f(glGetUniformLocation(s_Program->GetId(), "u_Position"), position.x, position.y);


    s_Program->Validate(); // 確保著色程序運行正常

    // 綁定並繪製血條
    s_VertexArray->Bind();
    s_VertexArray->DrawTriangles();
    s_Program->Unbind();
}
void Enemy::MovePosition(const glm::vec2& Position, const float totalTime) {
    MoveToPosition(GetPosition() + Position, totalTime);
}

void Enemy::MoveToPosition(const glm::vec2& targetPosition, const float totalTime) {
    if (totalTime <= 0.0f) {
        m_Transform.translation = targetPosition;
        return;
    }

    m_IsMoving = true;
    m_TargetPosition = targetPosition;
    m_MaxDistance = glm::distance(this->GetPosition(), targetPosition);
    m_Direction = (targetPosition - this->GetPosition()) / m_MaxDistance;
    m_Speed = m_MaxDistance / totalTime;
    m_TotalTime = totalTime * 1000.0f; //(ms)
    LOG_DEBUG("Move {} to {}",m_Name , m_Transform.translation);
}

void Enemy::Update() {
    // if (!m_IsMoving || !GetVisibility()) return;
    if (!m_IsMoving) return;
    // 計算移動距離
    const float DeltaTimeMs = Util::Time::GetDeltaTimeMs();
    const float moveDistance = m_Speed * DeltaTimeMs / 1000.0f;
    m_DistanceTraveled += moveDistance;
    // 更新位置
    m_Transform.translation += m_Direction * moveDistance;
    // 檢查是否到達目標距離
    if (m_DistanceTraveled >= m_MaxDistance) {
        m_IsMoving = false; // 停止移動
        m_Transform.translation = m_TargetPosition;
        m_DistanceTraveled = 0;
        LOG_DEBUG("{} move to {}",m_Name , m_Transform.translation);
    }
}
