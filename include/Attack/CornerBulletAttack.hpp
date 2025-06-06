#ifndef CORNERBULLETATTACK_HPP
#define CORNERBULLETATTACK_HPP

#include "Attack/CircleAttack.hpp"
#include <vector>
#include <random>

class CornerBulletAttack : public CircleAttack {
public:
    CornerBulletAttack(float delay, int bulletCount = 3, int sequenceNumber = 0);

    void SetBulletSpeed(float speed);
    void SetBulletCount(int count) { m_BulletCount = count; }

    void AddBulletPath(const glm::vec2& startPosition, float angle);

protected:
    void CreateWarningEffect() override;
    void CreateAttackEffect() override;
    void OnAttackStart() override;
    void CleanupVisuals() override;

private:
    std::vector<float> GenerateRandomAngles(float base);

    struct BulletPath {
        glm::vec2 startPosition;      // 發射起點
        glm::vec2 currentPosition;    // 當前位置 (用於碰撞檢測)
        float angle;                  // 發射角度（弧度）
        std::shared_ptr<Effect::CompositeEffect> warningEffect;  // 軌跡警告效果
        std::shared_ptr<CircleAttack> bulletAttack;   // 子彈攻擊 (使用 CircleAttack)
        bool isActive = false;        // 子彈是否活躍
    };

    std::vector<BulletPath> m_BulletPaths;
    float m_BulletSpeed = 350.0f;
    int m_BulletCount = 3;
    std::default_random_engine m_RandomEngine;
    std::vector<std::shared_ptr<CircleAttack>> m_BulletAttacks;
};

#endif