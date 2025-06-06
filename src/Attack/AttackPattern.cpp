#include "Attack/AttackPattern.hpp"
#include "Attack/AttackManager.hpp"
#include "Util/Logger.hpp"
#include <algorithm>

AttackPattern::AttackPattern() {}

void AttackPattern::AddAttack(std::shared_ptr<Attack> attack, float startTime) {
    // 新增攻擊到列表中
    m_Attacks.push_back({attack, startTime, false});

    // 根據開始時間排序
    std::sort(m_Attacks.begin(), m_Attacks.end(),
              [](const AttackItem& a, const AttackItem& b) {
                  return a.startTime < b.startTime;
              });

    // 更新總持續時間
    float attackEndTime = startTime + attack->GetDelay() + 0.5f;
    if (attackEndTime > m_TotalDuration) {
        m_TotalDuration = attackEndTime;
    }
}

void AttackPattern::AddEnemyMovement(const EnemyMovement& movement, float startTime, float duration) {
    m_Movements.push_back({movement, startTime, false, duration});

    std::sort(m_Movements.begin(), m_Movements.end(),
              [](const MovementItem& a, const MovementItem& b) {
                  return a.startTime < b.startTime;
              });
    float movementEndTime = startTime + duration;
    if (movementEndTime > m_TotalDuration) {
        m_TotalDuration = movementEndTime;
    }
}

void AttackPattern::Start(std::shared_ptr<Enemy> &enemy) {
    if (m_State != State::IDLE) return;

    m_Enemy = enemy;
    m_State = State::RUNNING;
    m_ElapsedTime = 0.0f;

    for (auto& item : m_Attacks) {item.started = false;}
    for (auto& item : m_Movements) {item.executed = false;}
}

void AttackPattern::Stop() {
    if (m_State != State::RUNNING) return;

    m_State = State::FINISHED;
}

void AttackPattern::Update(float deltaTime, std::shared_ptr<Character> player) {
    if (m_State != State::RUNNING) return;
    m_ElapsedTime += deltaTime;

    if (m_ElapsedTime >= m_TotalDuration) {
        m_State = State::FINISHED;
        return;
    }

    for (auto& item : m_Attacks) {
        if (!item.started && m_ElapsedTime >= item.startTime) {
            item.started = true;
            item.attack->SetTargetCharacter(player);
            AttackManager::GetInstance().RegisterAttack(item.attack);
        }
    }

    for (auto& item : m_Movements) {
        if (!item.executed && m_ElapsedTime >= item.startTime) {
            item.executed = true;
            if (m_Enemy) {
                item.movement(m_Enemy, item.duration);
            }
        }
    }
}