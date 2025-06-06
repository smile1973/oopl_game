#include "Attack/AttackManager.hpp"
#include "Util/Logger.hpp"

AttackManager& AttackManager::GetInstance() {
    static AttackManager instance;
    return instance;
}

void AttackManager::RegisterAttack(std::shared_ptr<Attack> attack) {
    if (!attack) return;
    m_ActiveAttacks.push_back(attack);
}

void AttackManager::Update(float deltaTime, std::shared_ptr<Character> &player) {
    // 更新所有攻擊並檢測碰撞
    for (auto it = m_ActiveAttacks.begin(); it != m_ActiveAttacks.end();) {
        auto& attack = *it;

        attack->Update(deltaTime);
        if (player && attack->GetState() == Attack::State::ATTACKING) {
            attack->CheckCollision(player);
        }
        // 如果攻擊已完成 從活躍列表中移除
        if (attack->IsFinished()) {
            it = m_ActiveAttacks.erase(it);
        } else {
            ++it;
        }
    }
}

void AttackManager::ClearAllAttacks() {
    for (auto& attack : m_ActiveAttacks) {
        if (attack) {
            attack->CleanupVisuals();
        }
    }
    m_ActiveAttacks.clear();
}