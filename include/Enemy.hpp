#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Character.hpp"
#include <vector>

class Enemy : public Character {
public:
    explicit Enemy(float health, float speed, const std::vector<std::string>& ImageSet);

    [[nodiscard]] float GetHealth() const { return m_Health; }
    void TakeDamage(float damage);
    bool IsAlive();

private:
    float m_Health;
    float m_Speed;
};

#endif // ENEMY_HPP
