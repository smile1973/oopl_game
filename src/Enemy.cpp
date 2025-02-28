#include "Enemy.hpp"
#include "Util/Logger.hpp"

Enemy::Enemy(float health, float speed, const std::vector<std::string>& ImageSet)
    : Character(ImageSet), m_Health(health), m_Speed(speed) {}

void Enemy::TakeDamage(float damage) {
    m_Health -= damage;
    LOG_DEBUG("Enemy took damage, remaining health: {}", m_Health);
}
