#include "Enemy.hpp"

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
    }
}

void Enemy::SetHealth(const float Health) {
    m_Health = (Health == -1.0f) ? m_MaxHealth : Health;
    m_MaxHealth = (Health == -1.0f) ? m_MaxHealth : Health;
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
    m_MaxDistance = glm::distance(this->GetPosition(), targetPosition);
    m_Direction = (targetPosition - this->GetPosition()) / m_MaxDistance;
    m_Speed = m_MaxDistance / totalTime;
}

void Enemy::Update() {
    if (!m_IsMoving || !GetVisibility()) return;
    // 計算移動距離
    const float moveDistance = m_Speed * Util::Time::GetDeltaTimeMs() / 1000.0f;
    m_DistanceTraveled += moveDistance;
    // 更新位置
    m_Transform.translation += m_Direction * moveDistance;
    // 檢查是否到達目標距離
    if (m_DistanceTraveled >= m_MaxDistance) {
        m_IsMoving = false; // 停止移動
        m_DistanceTraveled = 0;
        LOG_DEBUG("{} move to {}",m_Name , m_Transform.translation);
    }
}