#include "Character.hpp"
#include "Util/Renderer.hpp"
#include "Util/Logger.hpp"

#include <glm/gtx/norm.hpp>
#include <algorithm>

bool Character::IfCollide(const std::shared_ptr<Character>& other, const float Distance) const{
    if (!other) return false;
    // 取得當前角色和另一個角色的位置
    glm::vec2 pos1 = this->GetPosition();
    glm::vec2 pos2 = other->GetPosition();
    float size = Distance;
    // 簡單的 AABB (Axis-Aligned Bounding Box) 碰撞檢測
    bool isColliding = (abs(pos1.x - pos2.x) < size) && (abs(pos1.y - pos2.y) < size);
    return isColliding;
}

bool Character::IfCollideCircle(const std::shared_ptr<Character>& other, const float Distance) const {
    if (!other) return false;
    constexpr int enemyRadius = 150;
    // 判斷是否碰撞
    return glm::distance(this->GetPosition(), other->GetPosition()) < Distance + enemyRadius;
}

bool Character::IfCollideSweptCircle(const std::shared_ptr<Character>& other) const {
    if (!other) return false;

    const glm::vec2 pos1 = this->GetPosition();
    const glm::vec2 pos2 = other->GetPosition();

    const float distance = this->m_Transform.scale.x>0 ? 340.0f : -340.0f;
    constexpr float ballRadius = 60.0f;
    constexpr float enemyRadius = 150.0f;

    const glm::vec2 ballCenter = pos1 + glm::vec2(distance, 0);

    // 計算膠囊形狀的線段
    const glm::vec2 lineStart = pos1;
    const glm::vec2 lineEnd = ballCenter;

    // 計算敵人圓心到線段的最近點
    const glm::vec2 lineDirection = lineEnd - lineStart;
    const float lineLength = glm::length(lineDirection);
    const glm::vec2 lineNormalized = (lineLength > 0) ? lineDirection / lineLength : glm::vec2(0.0f);

    const float projection = glm::dot(pos2 - lineStart, lineNormalized);
    const float clampedProjection = std::clamp(projection, 0.0f, lineLength);
    const glm::vec2 closestPoint = lineStart + lineNormalized * clampedProjection;

    // 計算最近點到敵人圓心的距離
    const float collisionDistance = glm::distance(pos2, closestPoint);

    // 判斷是否碰撞
    return collisionDistance < ballRadius + enemyRadius;
}

bool Character::IfCollideEllipse(const std::shared_ptr<Character>& other) const {
    if (!other) return false;

    const glm::vec2 pos1 = this->GetPosition();
    const glm::vec2 pos2 = other->GetPosition();
    constexpr float enemyRadius = 150.0f; // 敵人半徑
    constexpr float rx = 275.0f + enemyRadius; // X 半徑
    constexpr float ry = 35.0f + enemyRadius; // Y 半徑

    // 敵人圓心相對於橢圓中心
    const glm::vec2 relativePos = pos2 - pos1;

    // 橢圓方程
    const float ellipseEquation = (relativePos.x * relativePos.x) / (rx * rx) +
                                  (relativePos.y * relativePos.y) / (ry * ry);
    // 如果敵人圓心在橢圓內部，則碰撞
    return ellipseEquation <= 1.0f;
}