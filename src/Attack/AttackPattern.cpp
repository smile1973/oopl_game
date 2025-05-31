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
    float attackEndTime = startTime + attack->GetDelay() + 0.5f; // 加上攻擊持續時間
    if (attackEndTime > m_TotalDuration) {
        m_TotalDuration = attackEndTime;
    }
}

void AttackPattern::AddEnemyMovement(const EnemyMovement& movement, float startTime, float duration) {
    // 新增敵人移動到列表中，加入持續時間
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

    // 更新經過的時間
    m_ElapsedTime += deltaTime;

    // 檢查是否達到總持續時間
    if (m_ElapsedTime >= m_TotalDuration) {
        m_State = State::FINISHED;
        return;
    }

    for (auto& item : m_Attacks) {
        if (!item.started && m_ElapsedTime >= item.startTime) {
            item.started = true;

            // 設置目標玩家
            item.attack->SetTargetCharacter(player);

            // 將攻擊註冊到攻擊管理器，後續更新由管理器處理
            AttackManager::GetInstance().RegisterAttack(item.attack);
        }
    }

    // 執行敵人移動
    for (auto& item : m_Movements) {
        // 檢查是否到達移動開始時間
        if (!item.executed && m_ElapsedTime >= item.startTime) {
            item.executed = true;

            // 執行移動函數，傳入持續時間參數
            if (m_Enemy) {
                item.movement(m_Enemy, item.duration);
            }
        }
    }
}