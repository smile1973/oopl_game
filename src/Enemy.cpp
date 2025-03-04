#include "Enemy.hpp"

#include <SDL.h>
#include "Util/Color.hpp"
#include "Util/Logger.hpp"
#include <GL/glew.h>

Enemy::Enemy(float health, float speed, const std::vector<std::string>& ImageSet)
    : Character(ImageSet), m_Health(health), m_Speed(speed) {}

void Enemy::TakeDamage(float damage) {
    m_Health -= damage;
    LOG_DEBUG("Enemy took {:.1f} damage, remaining health: {:.1f}", damage, m_Health);
}

bool Enemy::IsAlive() {
    return m_Health > 0;
}

