// include/Attack/EnemyAttackController.hpp
#ifndef ENEMYATTACKCONTROLLER_HPP
#define ENEMYATTACKCONTROLLER_HPP

#include "Attack/AttackPattern.hpp"
#include "Attack/AttackPatternFactory.hpp"
#include "Enemy.hpp"
#include "Character.hpp"
#include <queue>

/**
 * @class EnemyAttackController
 * @brief 管理敵人攻擊模式序列的控制器
 *
 * 此類別負責存儲和管理敵人的攻擊模式序列，
 * 讓敵人能夠按照預定義的順序執行不同的攻擊模式。
 */
class EnemyAttackController {
public:
    explicit EnemyAttackController(std::shared_ptr<Enemy> enemy);

    void InitBattle1Patterns();
    void InitBattle2Patterns();
    void InitBattle3Patterns();
    void InitBattle4Patterns();
    void InitBattle5Patterns();
    void InitBattle6Patterns();
    void InitBattle7Patterns();
    void InitBattle8Patterns();
    void InitBossPatterns();

    void Update(float deltaTime, std::shared_ptr<Character> player);
    
    bool IsAllPatternsCompleted() const;
    void AddPattern(std::shared_ptr<AttackPattern> pattern);
    void ClearPatterns();
    void Start();
    void Stop();
    void Reset();

    void SetCurrentPhase(int mainPhase, int subPhase) {
        m_CurrentMainPhase = mainPhase;
        m_CurrentSubPhase = subPhase;
    }

    void InitPatternsForCurrentPhase();

    int GetCurrentMainPhase() const { return m_CurrentMainPhase; }
    int GetCurrentSubPhase() const { return m_CurrentSubPhase; }

private:
    void SwitchToNextPattern();

    std::shared_ptr<Enemy> m_Enemy;
    std::queue<std::shared_ptr<AttackPattern>> m_PatternQueue;
    std::shared_ptr<AttackPattern> m_CurrentPattern;
    float m_CooldownTime = 2.0f;
    float m_ElapsedCooldownTime = 0.0f;
    bool m_IsInCooldown = false;
    bool m_IsActive = false;
    int m_CurrentMainPhase = 1;
    int m_CurrentSubPhase = 1;

    std::mt19937 m_RandomEngine;
    void SelectRandomPatternForBoss();
    std::vector<int> m_BossPatternTypes;
};

#endif