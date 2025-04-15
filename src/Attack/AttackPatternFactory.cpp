#include "Attack/AttackPatternFactory.hpp"
#include "Util/Logger.hpp"
#include <cmath>

AttackPatternFactory& AttackPatternFactory::GetInstance() {
    static AttackPatternFactory instance;
    return instance;
}

std::shared_ptr<AttackPattern> AttackPatternFactory::CreateSingleCirclePattern(
    const glm::vec2& position,
    float radius,
    float delay) {

    auto pattern = std::make_shared<AttackPattern>();
    auto attack = std::make_shared<CircleAttack>(position, delay, radius);
    pattern->AddAttack(attack, 0.0f);
    pattern->SetDuration(delay + 1.0f);
    return pattern;
}

std::shared_ptr<AttackPattern> AttackPatternFactory::CreateMultiCirclePattern(
    const std::vector<glm::vec2>& positions,
    float radius,
    float delay,
    float interval) {

    auto pattern = std::make_shared<AttackPattern>();

    // 創建多個圓形攻擊
    float startTime = 0.0f;
    int sequenceNumber = 1;

    for (const auto& position : positions) {
        auto attack = std::make_shared<CircleAttack>(position, delay, radius, sequenceNumber++);
        pattern->AddAttack(attack, startTime);
        startTime += interval;
    }
    pattern->SetDuration(startTime + delay + 1.0f);

    return pattern;
}

std::shared_ptr<AttackPattern> AttackPatternFactory::CreateRectanglePattern(
    const glm::vec2& position,
    float width,
    float height,
    float rotation,
    float delay) {

    auto pattern = std::make_shared<AttackPattern>();

    // 創建矩形攻擊
    auto attack = std::make_shared<RectangleAttack>(position, delay, width, height, rotation);

    // 將攻擊添加到模式中
    pattern->AddAttack(attack, 0.0f);

    // 設置模式總持續時間
    pattern->SetDuration(delay + 1.0f);

    return pattern;
}

std::shared_ptr<AttackPattern> AttackPatternFactory::CreateLaserPattern(
    const glm::vec2& position,
    RectangleAttack::Direction direction,
    float width,
    float length,
    float delay) {

    auto pattern = std::make_shared<AttackPattern>();

    // 創建雷射攻擊（使用整合後的RectangleAttack）
    auto attack = std::make_shared<RectangleAttack>(position, delay, direction, width, length);

    // 將攻擊添加到模式中
    pattern->AddAttack(attack, 0.0f);

    // 設置模式總持續時間
    pattern->SetDuration(delay + 1.0f);

    return pattern;
}

std::shared_ptr<AttackPattern> AttackPatternFactory::CreateMultiLaserPattern(
    const std::vector<glm::vec2>& positions,
    const std::vector<RectangleAttack::Direction>& directions,
    float width,
    float length,
    float delay,
    float interval) {

    auto pattern = std::make_shared<AttackPattern>();

    // 創建多個雷射攻擊
    float startTime = 0.0f;
    int sequenceNumber = 1;

    // 確保方向和位置數組長度相同，如果方向數組較短，循環使用
    size_t directionCount = directions.size();

    for (size_t i = 0; i < positions.size(); ++i) {
        const auto& position = positions[i];
        const auto& direction = directions[i % directionCount]; // 循環使用方向

        // 使用整合後的RectangleAttack創建雷射
        auto attack = std::make_shared<RectangleAttack>(
            position, delay, direction, width, length, sequenceNumber++
        );

        pattern->AddAttack(attack, startTime);
        startTime += interval;
    }

    // 設置模式總持續時間
    pattern->SetDuration(startTime + delay + 1.0f);

    return pattern;
}

std::vector<glm::vec2> AttackPatternFactory::CalculateCircularPositions(
    const glm::vec2& center,
    float radius,
    int count,
    float startAngle) {

    std::vector<glm::vec2> positions;
    positions.reserve(count);

    // 計算環上的每個點
    float angleStep = 2.0f * M_PI / static_cast<float>(count);

    for (int i = 0; i < count; ++i) {
        float angle = startAngle + i * angleStep;
        float x = center.x + radius * cos(angle);
        float y = center.y + radius * sin(angle);
        positions.emplace_back(x, y);
    }

    return positions;
}

std::shared_ptr<AttackPattern> AttackPatternFactory::CreateCircularPattern(
    const glm::vec2& centerPosition,
    float radius,
    float attackRadius,
    int count,
    float delay,
    float interval) {

    // 計算環上的攻擊位置
    std::vector<glm::vec2> positions = CalculateCircularPositions(centerPosition, radius, count);

    // 創建多圓攻擊模式
    auto pattern = CreateMultiCirclePattern(positions, attackRadius, delay, interval);

    pattern->AddEnemyMovement([centerPosition](std::shared_ptr<Enemy> enemy, float totalTime) {
        enemy->MoveToPosition(centerPosition, totalTime);
        LOG_DEBUG("Enemy moving to center position: ({}, {}), duration: {}",
                 centerPosition.x, centerPosition.y, totalTime);
    }, 0.0f, 1.5f);  // 設置開始時間為0，持續時間為1.5秒

    return pattern;
}

std::shared_ptr<AttackPattern> AttackPatternFactory::CreateCrossRotatingLaserPattern(
    const glm::vec2& centerPosition,
    float width,
    float height,
    float rotationSpeed,
    float duration,
    float delay) {
    auto pattern = std::make_shared<AttackPattern>();
    auto horizontalAttack = std::make_shared<RectangleAttack>(
        centerPosition, delay, width, height, 0.0f, 1
    );

    auto verticalAttack = std::make_shared<RectangleAttack>(
        centerPosition, delay, width, height, 1.57f, 2  // 1.57 rad ≈ 90度
    );

    horizontalAttack->SetAutoRotation(true, rotationSpeed);
    horizontalAttack->SetAttackDuration(duration);
    verticalAttack->SetAutoRotation(true, rotationSpeed);
    verticalAttack->SetAttackDuration(duration);
    pattern->AddAttack(horizontalAttack, 0.0f);
    pattern->AddAttack(verticalAttack, 0.0f);
    pattern->SetDuration(delay + duration + 1.0f);
    return pattern;
}

std::shared_ptr<AttackPattern> AttackPatternFactory::CreateMovingCirclePattern(
    const glm::vec2& startPosition,
    const glm::vec2& endPosition,
    float radius,
    float speed,
    float delay) {

    auto pattern = std::make_shared<AttackPattern>();
    auto circleAttack = std::make_shared<CircleAttack>(startPosition, delay, radius, 1);

    glm::vec2 direction = endPosition - startPosition;
    float distance = glm::length(direction);
    direction = glm::normalize(direction);

    float moveDuration = distance / speed;
    circleAttack->SetColor(Util::Color::FromRGB(255, 100, 0, 200));
    circleAttack->SetMovementParams(direction, speed, distance);
    circleAttack->SetAttackDuration(moveDuration + 0.5f);  // 加一點緩衝時間
    pattern->AddAttack(circleAttack, 0.0f);
    pattern->SetDuration(delay + moveDuration + 1.0f);

    return pattern;
}

std::shared_ptr<AttackPattern> AttackPatternFactory::CreateCornerBulletPattern(
    int bulletCount,
    float bulletSpeed,
    float bulletRadius,
    float delay) {

    auto pattern = std::make_shared<AttackPattern>();

    // 創建角落子彈攻擊
    auto attack = std::make_shared<CornerBulletAttack>(delay, bulletCount);
    attack->SetBulletSpeed(bulletSpeed);
    attack->SetRadius(bulletRadius);

    pattern->AddAttack(attack, 0.0f);
    pattern->SetDuration(delay + 3.0f);  // 給予足夠時間讓子彈飛行
    return pattern;
}

std::shared_ptr<AttackPattern> AttackPatternFactory::CreateBattle1Pattern() {
    auto pattern = std::make_shared<AttackPattern>();

    glm::vec2 centerPosition(200.0f, 0.0f);
    pattern->AddEnemyMovement([centerPosition](const std::shared_ptr<Enemy>& enemy, float totalTime) {
        enemy->MoveToPosition(centerPosition, totalTime);
    }, 0.0f, 1.5f);  // 設置開始時間為0，持續時間為1.5秒

    glm::vec2 startPos(680.0f, 0.0f);
    glm::vec2 endPos(-680.0f, 0.0f);
    auto attacka1 = std::make_shared<CircleAttack>(startPos, 2.0f, 200.0f);
    attacka1->SetColor(Util::Color(1.0, 0.4, 0.4, 0.7));
    attacka1->SetMovementParams(glm::normalize(endPos - startPos), 160.0f, glm::length(endPos - startPos));
    pattern->AddAttack(attacka1, 0);

    startPos = glm::vec2(0.0f, 360.0f);
    endPos = glm::vec2(0.0f, -360.0f);
    auto attacka2 = std::make_shared<CircleAttack>(startPos, 2.0f, 200.0f);
    attacka2->SetColor(Util::Color(1.0, 0.4, 0.4, 0.5));
    attacka2->SetMovementParams(glm::normalize(endPos - startPos), 160.0f, glm::length(endPos - startPos));
    pattern->AddAttack(attacka2, 0);

    for (int i = 0; i < 10; i++) {
        startPos = glm::vec2(-620.0f + static_cast<float>(i) * 128.0f, 360.0f);
        endPos = glm::vec2(-620.0f + static_cast<float>(i) * 128.0f, -360.0f);
        auto attack1 = std::make_shared<CircleAttack>(startPos, 1.0f, 32.0f, i + 1);
        attack1->SetColor(Util::Color(0.0, 1.0, 0.3, 0.4));
        attack1->SetMovementParams(glm::normalize(endPos - startPos), 350.0f, glm::length(endPos - startPos));
        auto attack2 = std::make_shared<CircleAttack>(startPos, 1.0f, 32.0f, i + 1);
        attack2->SetColor(Util::Color(0.0, 1.0, 0.3, 0.4));
        attack2->SetMovementParams(glm::normalize(endPos - startPos), 350.0f, glm::length(endPos - startPos));
        auto attack3 = std::make_shared<CircleAttack>(startPos, 1.0f, 32.0f, i + 1);
        attack3->SetColor(Util::Color(0.0, 1.0, 0.3, 0.4));
        attack3->SetMovementParams(glm::normalize(endPos - startPos), 350.0f, glm::length(endPos - startPos));
        pattern->AddAttack(attack1, 1.0);
        pattern->AddAttack(attack2, 2.0);
        pattern->AddAttack(attack3, 3.0);
    }

    startPos = glm::vec2(-680.0f, 160.0f);
    endPos = glm::vec2(680.0f, 160.0f);
    auto attackb1 = std::make_shared<CircleAttack>(startPos, 2.0f, 200.0f);
    attackb1->SetColor(Util::Color(1.0, 0.4, 0.4, 0.5));
    attackb1->SetMovementParams(glm::normalize(endPos - startPos), 160.0f, glm::length(endPos - startPos));
    pattern->AddAttack(attackb1, 5.0);

    startPos = glm::vec2(200.0f, 360.0f);
    endPos = glm::vec2(200.0f, -360.0f);
    auto attackb2 = std::make_shared<CircleAttack>(startPos, 2.0f, 200.0f);
    attackb2->SetColor(Util::Color(1.0, 0.4, 0.4, 0.5));
    attackb2->SetMovementParams(glm::normalize(endPos - startPos), 160.0f, glm::length(endPos - startPos));
    pattern->AddAttack(attackb2, 5.0);

    for (int i = 0; i < 10; i++) {
        startPos = glm::vec2(-620.0f + static_cast<float>(i) * 128.0f, -360.0f);
        endPos = glm::vec2(-620.0f + static_cast<float>(i) * 128.0f, 360.0f);
        auto attack1 = std::make_shared<CircleAttack>(startPos, 1.0f, 32.0f, i + 1);
        attack1->SetColor(Util::Color(0.0, 1.0, 0.3, 0.4));
        attack1->SetMovementParams(glm::normalize(endPos - startPos), 350.0f, glm::length(endPos - startPos));
        auto attack2 = std::make_shared<CircleAttack>(startPos, 1.0f, 32.0f, i + 1);
        attack2->SetColor(Util::Color(0.0, 1.0, 0.3, 0.4));
        attack2->SetMovementParams(glm::normalize(endPos - startPos), 350.0f, glm::length(endPos - startPos));
        auto attack3 = std::make_shared<CircleAttack>(startPos, 1.0f, 32.0f, i + 1);
        attack3->SetColor(Util::Color(0.0, 1.0, 0.3, 0.4));
        attack3->SetMovementParams(glm::normalize(endPos - startPos), 350.0f, glm::length(endPos - startPos));
        pattern->AddAttack(attack1, 6.0);
        pattern->AddAttack(attack2, 7.0);
        pattern->AddAttack(attack3, 8.0);
    }

    glm::vec2 upPosition(200.0f, 100.0f);
    pattern->AddEnemyMovement([upPosition](const std::shared_ptr<Enemy>& enemy, float totalTime) {
        enemy->MoveToPosition(upPosition, totalTime);
    }, 9.5f, 1.5f);  // 設置開始時間為0，持續時間為1.5秒

    startPos = glm::vec2(680.0f, 0.0f);
    endPos = glm::vec2(-680.0f, 0.0f);
    auto attackc1 = std::make_shared<CircleAttack>(startPos, 2.0f, 200.0f);
    attackc1->SetColor(Util::Color(1.0, 0.4, 0.4, 0.5));
    attackc1->SetMovementParams(glm::normalize(endPos - startPos), 160.0f, glm::length(endPos - startPos));
    pattern->AddAttack(attackc1, 11.0);

    startPos = glm::vec2(0.0f, 360.0f);
    endPos = glm::vec2(0.0f, -360.0f);
    auto attackc2 = std::make_shared<CircleAttack>(startPos, 2.0f, 200.0f);
    attackc2->SetColor(Util::Color(1.0, 0.4, 0.4, 0.5));
    attackc2->SetMovementParams(glm::normalize(endPos - startPos), 160.0f, glm::length(endPos - startPos));
    pattern->AddAttack(attackc2, 11.0);

    for (int i = 0; i < 10; i++) {
        startPos = glm::vec2(-620.0f + static_cast<float>(i) * 128.0f, 360.0f);
        endPos = glm::vec2(-620.0f + static_cast<float>(i) * 128.0f, -360.0f);
        auto attack1 = std::make_shared<CircleAttack>(startPos, 1.0f, 32.0f, i + 1);
        attack1->SetColor(Util::Color(0.0, 1.0, 0.3, 0.4));
        attack1->SetMovementParams(glm::normalize(endPos - startPos), 350.0f, glm::length(endPos - startPos));
        auto attack2 = std::make_shared<CircleAttack>(startPos, 1.0f, 32.0f, i + 1);
        attack2->SetColor(Util::Color(0.0, 1.0, 0.3, 0.4));
        attack2->SetMovementParams(glm::normalize(endPos - startPos), 350.0f, glm::length(endPos - startPos));
        auto attack3 = std::make_shared<CircleAttack>(startPos, 1.0f, 32.0f, i + 1);
        attack3->SetColor(Util::Color(0.0, 1.0, 0.3, 0.4));
        attack3->SetMovementParams(glm::normalize(endPos - startPos), 350.0f, glm::length(endPos - startPos));
        pattern->AddAttack(attack1, 12.0);
        pattern->AddAttack(attack2, 13.0);
        pattern->AddAttack(attack3, 14.0);
    }

    startPos = glm::vec2(-680.0f, 0.0f);
    endPos = glm::vec2(680.0f, 0.0f);
    auto attackd1 = std::make_shared<CircleAttack>(startPos, 2.0f, 200.0f);
    attackd1->SetColor(Util::Color(1.0, 0.4, 0.4, 0.5));
    attackd1->SetMovementParams(glm::normalize(endPos - startPos), 160.0f, glm::length(endPos - startPos));
    pattern->AddAttack(attackd1, 16.0);

    startPos = glm::vec2(-200.0f, 360.0f);
    endPos = glm::vec2(-200.0f, -360.0f);
    auto attackd2 = std::make_shared<CircleAttack>(startPos, 2.0f, 200.0f);
    attackd2->SetColor(Util::Color(1.0, 0.4, 0.4, 0.5));
    attackd2->SetMovementParams(glm::normalize(endPos - startPos), 160.0f, glm::length(endPos - startPos));
    pattern->AddAttack(attackd2, 16.0);

    for (int i = 0; i < 10; i++) {
        startPos = glm::vec2(-620.0f + static_cast<float>(i) * 128.0f, -360.0f);
        endPos = glm::vec2(-620.0f + static_cast<float>(i) * 128.0f, 360.0f);
        auto attack1 = std::make_shared<CircleAttack>(startPos, 1.0f, 32.0f, i + 1);
        attack1->SetColor(Util::Color(0.0, 1.0, 0.3, 0.4));
        attack1->SetMovementParams(glm::normalize(endPos - startPos), 350.0f, glm::length(endPos - startPos));
        auto attack2 = std::make_shared<CircleAttack>(startPos, 1.0f, 32.0f, i + 1);
        attack2->SetColor(Util::Color(0.0, 1.0, 0.3, 0.4));
        attack2->SetMovementParams(glm::normalize(endPos - startPos), 350.0f, glm::length(endPos - startPos));
        auto attack3 = std::make_shared<CircleAttack>(startPos, 1.0f, 32.0f, i + 1);
        attack3->SetColor(Util::Color(0.0, 1.0, 0.3, 0.4));
        attack3->SetMovementParams(glm::normalize(endPos - startPos), 350.0f, glm::length(endPos - startPos));
        pattern->AddAttack(attack1, 17.0);
        pattern->AddAttack(attack2, 18.0);
        pattern->AddAttack(attack3, 19.0);
    }

    pattern->SetDuration(22.0f);
    return pattern;
}

std::shared_ptr<AttackPattern> AttackPatternFactory::CreateBattle2Pattern() {
    auto pattern = std::make_shared<AttackPattern>();
    glm::vec2 centerPosition(0.0f, 0.0f);
    pattern->AddEnemyMovement([centerPosition](const std::shared_ptr<Enemy>& enemy, float totalTime) {
        enemy->MoveToPosition(centerPosition, totalTime);
    }, 0.0f, 1.0f);  // 設置開始時間為0，持續時間為1.5秒

    float delay = 1.5f;
    auto cornerbulletAttack = std::make_shared<CornerBulletAttack>(delay, 3);
    cornerbulletAttack->SetBulletSpeed(900.0f);
    cornerbulletAttack->SetRadius(35.0f);
    pattern->AddAttack(cornerbulletAttack, 1.0f);

    float duration = 3.0f;
    auto rotateAttack = std::make_shared<RectangleAttack>(
        centerPosition, delay, 1500.0f, 100.0f, 2.0f, 1
    );
    float rotationSpeed = 0.35f;
    rotateAttack->SetAutoRotation(true, rotationSpeed);
    rotateAttack->SetAttackDuration(duration);
    pattern->AddAttack(rotateAttack, 3.5f);
    auto circleAttack = std::make_shared<CircleAttack>(centerPosition, delay, 250.0f, 1);
    circleAttack->SetColor(Util::Color(1.0, 0.0, 0.3, 0.4));
    circleAttack->SetAttackDuration(duration);
    pattern->AddAttack(circleAttack, 3.5f);


    auto cornerbulletAttack2 = std::make_shared<CornerBulletAttack>(delay, 3);
    cornerbulletAttack2->SetBulletSpeed(900.0f);
    cornerbulletAttack2->SetRadius(35.0f);
    pattern->AddAttack(cornerbulletAttack2, 9.0f);
    rotationSpeed = -0.35f;
    auto rotateAttack2 = std::make_shared<RectangleAttack>(
        centerPosition, delay, 1500.0f, 100.0f, -2.0f, 1
    );
    rotateAttack2->SetAutoRotation(true, rotationSpeed);
    rotateAttack2->SetAttackDuration(duration);
    pattern->AddAttack(rotateAttack2, 11.5f);
    auto circleAttack2 = std::make_shared<CircleAttack>(centerPosition, delay, 250.0f, 1);
    circleAttack2->SetColor(Util::Color(1.0, 0.0, 0.3, 0.4));
    circleAttack2->SetAttackDuration(duration);
    pattern->AddAttack(circleAttack2, 11.5f);

    pattern->SetDuration(16.0f);
    return pattern;
}

std::shared_ptr<AttackPattern> AttackPatternFactory::CreateBattle3Pattern() {
    auto pattern = std::make_shared<AttackPattern>();
    glm::vec2 centerPosition(0.0f, 0.0f);
    pattern->AddEnemyMovement([centerPosition](const std::shared_ptr<Enemy>& enemy, float totalTime) {
        enemy->MoveToPosition(centerPosition, totalTime);
    }, 0.0f, 1.0f);

    float delay = 2.0f;
    glm::vec2 startPos;
    glm::vec2 endPos;
    for (int i = 0; i < 11; i++) {
        startPos = glm::vec2(620.0f, 340.0f - static_cast<float>(i) * 30.0f);
        endPos = glm::vec2(-620.0f, 340.0f - static_cast<float>(i) * 30.0f);
        auto attack1 = std::make_shared<CircleAttack>(startPos, 1.0f, 20.0f, i + 1);
        attack1->SetColor(Util::Color(0.0, 1.0, 1.0, 0.4));
        attack1->SetMovementParams(glm::normalize(endPos - startPos), 350.0f, glm::length(endPos - startPos));
        auto attack2 = std::make_shared<CircleAttack>(startPos, 1.0f, 20.0f, i + 1);
        attack2->SetColor(Util::Color(0.0, 0.0, 1.0, 0.4));
        attack2->SetMovementParams(glm::normalize(endPos - startPos), 350.0f, glm::length(endPos - startPos));
        auto attack3 = std::make_shared<CircleAttack>(startPos, 1.0f, 20.0f, i + 1);
        attack3->SetColor(Util::Color(0.0, 0.0, 1.0, 0.4));
        attack3->SetMovementParams(glm::normalize(endPos - startPos), 350.0f, glm::length(endPos - startPos));
        pattern->AddAttack(attack1, 1.0);
        pattern->AddAttack(attack2, 2.0);
        pattern->AddAttack(attack3, 5.0);
    }
    for (int i = 0; i < 11; i++) {
        startPos = glm::vec2(-620.0f, -340.0f + static_cast<float>(i) * 30.0f);
        endPos = glm::vec2(620.0f, -340.0f + static_cast<float>(i) * 30.0f);
        auto attack1 = std::make_shared<CircleAttack>(startPos, 1.0f, 20.0f, i + 1);
        attack1->SetColor(Util::Color(0.0, 1.0, 1.0, 0.4));
        attack1->SetMovementParams(glm::normalize(endPos - startPos), 350.0f, glm::length(endPos - startPos));
        auto attack2 = std::make_shared<CircleAttack>(startPos, 1.0f, 20.0f, i + 1);
        attack2->SetColor(Util::Color(0.0, 0.0, 1.0, 0.4));
        attack2->SetMovementParams(glm::normalize(endPos - startPos), 350.0f, glm::length(endPos - startPos));
        auto attack3 = std::make_shared<CircleAttack>(startPos, 1.0f, 20.0f, i + 1);
        attack3->SetColor(Util::Color(0.0, 0.0, 1.0, 0.4));
        attack3->SetMovementParams(glm::normalize(endPos - startPos), 350.0f, glm::length(endPos - startPos));
        pattern->AddAttack(attack1, 3.0);
        pattern->AddAttack(attack2, 4.0);
        pattern->AddAttack(attack3, 6.0);
    }

    for (int i = 0; i < 3; i++) {
        startPos = glm::vec2(-600.0f + static_cast<float>(i) * 600.0f, 360.0f);
        endPos = glm::vec2(-600.0f + static_cast<float>(i) * 600.0f, -360.0f);
        auto attack1 = std::make_shared<CircleAttack>(startPos, delay, 230.0f, i + 1);
        attack1->SetColor(Util::Color(1.0, 0.0, 0.3, 0.4));
        attack1->SetMovementParams(glm::normalize(endPos - startPos), 220.0f, glm::length(endPos - startPos));
        pattern->AddAttack(attack1, 3.0);
    }
    for (int i = 0; i < 2; i++) {
        startPos = glm::vec2(-300.0f + static_cast<float>(i) * 600.0f, 360.0f);
        endPos = glm::vec2(-300.0f + static_cast<float>(i) * 600.0f, -360.0f);
        auto attack1 = std::make_shared<CircleAttack>(startPos, delay, 230.0f, i + 1);
        attack1->SetColor(Util::Color(1.0, 0.0, 0.3, 0.4));
        attack1->SetMovementParams(glm::normalize(endPos - startPos), 220.0f, glm::length(endPos - startPos));
        pattern->AddAttack(attack1, 6.0);
    }
    pattern->SetDuration(7.0f);
    return pattern;
}
