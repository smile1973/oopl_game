#ifndef ATTACKMANAGER_HPP
#define ATTACKMANAGER_HPP

#include <memory>
#include <vector>
#include "Attack/Attack.hpp"
#include "Character.hpp"

/**
 * @class AttackManager
 * @brief 管理所有遊戲中的攻擊物件
 * 
 * 此類別負責集中管理所有活躍的攻擊物件，處理攻擊的更新和碰撞檢測。
 * 攻擊模式只負責生成攻擊，生成後的管理由此類別負責。
 */
class AttackManager {
public:
    static AttackManager& GetInstance();

    AttackManager(const AttackManager&) = delete;
    AttackManager& operator=(const AttackManager&) = delete;

    void RegisterAttack(std::shared_ptr<Attack> attack);
    void Update(float deltaTime, std::shared_ptr<Character> &player);
    void ClearAllAttacks();
    size_t GetActiveAttacksCount() const { return m_ActiveAttacks.size(); }

private:
    AttackManager() = default;

    std::vector<std::shared_ptr<Attack>> m_ActiveAttacks;
};

#endif // ATTACKMANAGER_HPP