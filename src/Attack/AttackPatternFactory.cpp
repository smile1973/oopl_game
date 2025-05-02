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

void AttackPatternFactory::AddCircleAttackRow(
    std::shared_ptr<AttackPattern> pattern,
    const glm::vec2& startPos,
    bool isHorizontal,
    int count,
    float zind,
    float spacing,
    float radius,
    const Util::Color& color,
    const glm::vec2& moveDirection,
    float moveSpeed,
    float moveDistance,
    float delay,
    float startTime,
    float timeInterval) {

    for (int i = 0; i < count; i++) {
        // Calculate position based on direction
        glm::vec2 position = startPos;
        if (isHorizontal) {
            position.x += static_cast<float>(i) * spacing;
        } else {
            position.y += static_cast<float>(i) * spacing;
        }

        // Create attack
        auto attack = std::make_shared<CircleAttack>(position, delay, radius, i + 1);
        attack->SetColor(color);
        attack->SetZ(zind);

        // Set movement if applicable
        if (moveSpeed > 0.0f) {
            attack->SetMovementParams(moveDirection, moveSpeed, moveDistance);
        }
        pattern->AddAttack(attack, startTime + static_cast<float>(i) * timeInterval);
    }
}

void AttackPatternFactory::AddCrossLaserAttack(
    std::shared_ptr<AttackPattern> pattern,
    const glm::vec2& centerPosition,
    float width,
    float length,
    const Util::Color& color,
    float delay,
    float startTime,
    float duration,
    int sequenceNumber) {

    // 建立水平雷射
    auto horizontalLaser = std::make_shared<RectangleAttack>(
        centerPosition, delay, width, length, 0.0f, sequenceNumber
    );
    horizontalLaser->SetColor(color);
    horizontalLaser->SetAttackDuration(duration);
    horizontalLaser->SetAutoRotation(false); // 確保不旋轉

    // 建立垂直雷射
    auto verticalLaser = std::make_shared<RectangleAttack>(
        centerPosition, delay, width, length, 1.57f, sequenceNumber + 1  // 1.57 rad ≈ 90度
    );
    verticalLaser->SetColor(color);
    verticalLaser->SetAttackDuration(duration);
    verticalLaser->SetAutoRotation(false); // 確保不旋轉

    // 添加到模式
    horizontalLaser->SetZ(10.0);
    verticalLaser->SetZ(10.0);
    pattern->AddAttack(horizontalLaser, startTime);
    pattern->AddAttack(verticalLaser, startTime);
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


    for (int wave = 0; wave < 3; wave++) {
        AddCircleAttackRow(
            pattern,                   // Pattern to add to
            glm::vec2(-620.0f, 360.0f), // Starting position (top left)
            true,                      // Horizontal row
            10,                        // 10 circles
            20.0,                  // zindex
            128.0f,                    // 128px spacing
            32.0f,                     // 32px radius
            Util::Color(0.0, 1.0, 0.3, 0.4), // Green color
            glm::vec2(0.0f, -1.0f),    // Move downward
            350.0f,                    // Speed
            720.0f,                    // Distance (full height)
            1.0f,                      // 1s delay
            1.0f + wave,               // Start at 6 7 8
            0.0f                       // No interval (simultaneous)
        );
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

    for (int wave = 0; wave < 3; wave++) {
        AddCircleAttackRow(
            pattern,                   // Pattern to add to
            glm::vec2(-620.0f, 360.0f), // Starting position (top left)
            true,                      // Horizontal row
            10,                        // 10 circles
            20.0,                  // zindex
            128.0f,                    // 128px spacing
            32.0f,                     // 32px radius
            Util::Color(0.0, 1.0, 0.3, 0.4), // Green color
            glm::vec2(0.0f, -1.0f),    // Move downward
            350.0f,                    // Speed
            720.0f,                    // Distance (full height)
            1.0f,                      // 1s delay
            6.0f + wave,               // Start at 6 7 8
            0.0f                       // No interval (simultaneous)
        );
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

    for (int wave = 0; wave < 3; wave++) {
        AddCircleAttackRow(
            pattern,                    // Pattern to add to
            glm::vec2(-620.0f, -360.0f), // Starting position (bottom left)
            true,                       // Horizontal row
            10,                         // 10 circles
            20.0f,
            128.0f,                     // 128px spacing
            32.0f,                      // 32px radius
            Util::Color(0.0, 1.0, 0.3, 0.4), // Green color
            glm::vec2(0.0f, 1.0f),      // Move upward
            350.0f,                     // Speed
            720.0f,                     // Distance (full height)
            1.0f,                       // 1s delay
            12.0f + wave,                // Start at 12 13 14
            0.0f                        // No interval (simultaneous)
        );
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

    for (int wave = 0; wave < 3; wave++) {
        AddCircleAttackRow(
            pattern,                   // Pattern to add to
            glm::vec2(-620.0f, 360.0f), // Starting position (top left)
            true,                      // Horizontal row
            10,                        // 10 circles
            20.0f,
            128.0f,                    // 128px spacing
            32.0f,                     // 32px radius
            Util::Color(0.0, 1.0, 0.3, 0.4), // Green color
            glm::vec2(0.0f, -1.0f),    // Move downward
            350.0f,                    // Speed
            720.0f,                    // Distance (full height)
            1.0f,                      // 1s delay
            17.0f + wave,              // 17 18 19
            0.0f                       // No interval (simultaneous)
        );
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
    for (int wave = 0; wave < 3; wave++) {
        AddCircleAttackRow(
            pattern,                   // Pattern to add to
            glm::vec2(620.0f, 360.0f), // Starting position (top right)
            false,
            11,
            20.0,
            -30.0f,
            20.0f,
            Util::Color(0.0, 1.0, 0.3, 0.4),
            glm::vec2(-1.0f, 0.0f),
            350.0f,                    // Speed
            1200.0f,                    // Distance
            1.0f,                      // 1s delay
            1.0f + wave * 2.0,               // Start at 6 7 8
            0.0f                       // No interval (simultaneous)
        );
    }
    for (int wave = 0; wave < 3; wave++) {
        AddCircleAttackRow(
            pattern,                   // Pattern to add to
            glm::vec2(-620.0f, -360.0f), // Starting position (top left)
            false,
            11,
            20.0,
            30.0f,
            20.0f,
            Util::Color(0.0, 1.0, 0.3, 0.4),
            glm::vec2(1.0f, 0.0f),
            350.0f,                    // Speed
            1200.0f,                    // Distance
            1.0f,                      // 1s delay
            2.0f + wave * 2.0,               // Start at 6 7 8
            0.0f                       // No interval (simultaneous)
        );
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

std::shared_ptr<AttackPattern> AttackPatternFactory::CreateBattle4Pattern() {
    auto pattern = std::make_shared<AttackPattern>();
    glm::vec2 centerPosition(0.0f, 0.0f);
    pattern->AddEnemyMovement([centerPosition](const std::shared_ptr<Enemy>& enemy, float totalTime) {
        enemy->MoveToPosition(centerPosition, totalTime);
    }, 0.0f, 1.0f);
    centerPosition = {-400.0f, 0.0f};
    pattern->AddEnemyMovement([centerPosition](const std::shared_ptr<Enemy>& enemy, float totalTime) {
        enemy->MoveToPosition(centerPosition, totalTime);
    }, 6.0f, 1.0f);

    std::vector<glm::vec2> pos1 = {{-600.0f, -400.0f}, {300.0f, -50.0f}, {600.0f, 400.0f}, {60.0f, 190.0f}};
    std::vector<glm::vec2> pos2 = {{-600.0f, -150.0f}, {630.0f, -300.0f}, {-200.0f, 180.0f}, {0.0f, 340.0f}};
    for (int count = 0; count < 4; count++) {
        AddCrossLaserAttack(
            pattern,                       // 攻擊模式
            pos1[count],   // 中心位置
            200.0f,                         // 寬度
            2500.0f,                       // 長度
            Util::Color(1.0, 0.8, 0.0, 0.6), // 黃色
            2.0f,                          // 倒數時間
            2.0f,            // 開始時間
            1.0f                           // 持續時間
        );
        AddCrossLaserAttack(
            pattern,                       // 攻擊模式
            pos2[count],   // 中心位置
            200.0f,                         // 寬度
            2500.0f,                       // 長度
            Util::Color(1.0, 0.8, 0.0, 0.6), // 黃色
            2.0f,                          // 倒數時間
            7.0f,            // 開始時間
            1.0f                           // 持續時間
        );
    }

    centerPosition = {0.0f, 0.0f};
    pattern->AddEnemyMovement([centerPosition](const std::shared_ptr<Enemy>& enemy, float totalTime) {
        enemy->MoveToPosition(centerPosition, totalTime);
    }, 10.5f, 1.0f);
    float duration = 1.0f;
    float delay = 2.5f;
    auto rotateAttack = std::make_shared<RectangleAttack>(
        centerPosition, delay, 1500.0f, 150.0f, 0.0f, 1
    );
    float rotationSpeed = 0.35f;
    rotateAttack->SetAutoRotation(true, rotationSpeed);
    rotateAttack->SetAttackDuration(duration);
    pattern->AddAttack(rotateAttack, 12.0f);

    auto rotateAttack2 = std::make_shared<RectangleAttack>(
        centerPosition, delay, 1500.0f, 150.0f, 0.0f, 1
    );
    rotationSpeed = 0.35f;
    rotateAttack2->SetAutoRotation(true, rotationSpeed);
    rotateAttack2->SetAttackDuration(duration);
    pattern->AddAttack(rotateAttack2, 18.0f);

    std::vector<glm::vec2> pos3 = {{500.0f, -400.0f}, {300.0f, 400.0f}, {100.0f, -400.0f}, {-100.0f, 400.0f},{-550.0, -400.0f}};
    std::vector<glm::vec2> pos4 = {{-500.0f, -400.0f}, {-300.0f, 400.0f}, {-100.0f, -400.0f}, {350.0f, 400.0f},{550.0, -400.0f}};
    for (int count = 0; count < 5; count++) {
        AddCrossLaserAttack(
            pattern,                       // 攻擊模式
            pos3[count],   // 中心位置
            250.0f,                         // 寬度
            2500.0f,                       // 長度
            Util::Color(1.0, 0.8, 0.0, 0.6), // 黃色
            2.0f,                          // 倒數時間
            12.5f + (count * 0.3f),            // 開始時間
            1.0f                           // 持續時間
        );
        AddCrossLaserAttack(
            pattern,                       // 攻擊模式
            pos4[count],   // 中心位置
            250.0f,                         // 寬度
            2500.0f,                       // 長度
            Util::Color(1.0, 0.8, 0.0, 0.6), // 黃色
            2.0f,                          // 倒數時間
            18.5f + (count * 0.3f),            // 開始時間
            1.0f                           // 持續時間
        );
    }

    centerPosition = {300.0f, 0.0f};
    auto rotateAttack3 = std::make_shared<RectangleAttack>(
        centerPosition, delay, 2500.0f, 150.0f, 0.0f, 1
    );
    rotationSpeed = 0.35f;
    rotateAttack3->SetAutoRotation(true, rotationSpeed);
    rotateAttack3->SetAttackDuration(duration);
    pattern->AddAttack(rotateAttack3, 0.0f);


    pattern->SetDuration(23.0f);
    return pattern;
}

std::shared_ptr<AttackPattern> AttackPatternFactory::CreateBattle5Pattern() {
    auto pattern = std::make_shared<AttackPattern>();
    glm::vec2 position(0.0f, 0.0f);
    pattern->AddEnemyMovement([position](const std::shared_ptr<Enemy>& enemy, float totalTime) {
        enemy->MoveToPosition(position, totalTime);
    }, 0.0f, 1.0f);
    position = glm::vec2(-300.0, -100.0);
    pattern->AddEnemyMovement([position](const std::shared_ptr<Enemy>& enemy, float totalTime) {
        enemy->MoveToPosition(position, totalTime);
    }, 5.0f, 2.0f);
    pattern->AddEnemyMovement([position](const std::shared_ptr<Enemy>& enemy, float totalTime) {
        enemy->MoveToPosition(position, totalTime);
    }, 15.0f, 2.0f);
    position = glm::vec2(300.0, 100.0);
    pattern->AddEnemyMovement([position](const std::shared_ptr<Enemy>& enemy, float totalTime) {
        enemy->MoveToPosition(position, totalTime);
    }, 10.0f, 2.0f);
    pattern->AddEnemyMovement([position](const std::shared_ptr<Enemy>& enemy, float totalTime) {
        enemy->MoveToPosition(position, totalTime);
    }, 20.0f, 2.0f);

    for (int wave = 0; wave < 6; wave++) {
        AddCircleAttackRow(
            pattern,                   // Pattern to add to
            glm::vec2(640.0f, 360.0f), // top right
            true,                    // vertical row
            20,                        // 10 circles
            20.0,                     // zindex
            -65.0f,                   // spacing
            15.0f,                    // radius
            Util::Color(0.0, 1.0, 1.0, 0.4),
            glm::vec2(0.0f, -1.0f),// Move left
            110.0f,                    // Speed
            1200.0f,                   // Distance
            1.0f,                      // delay
            1.0f + wave * 3.8f,        // Start at
            0.0f                       // No interval (simultaneous)
        );
        AddCircleAttackRow(
            pattern,                   // Pattern to add to
            glm::vec2(685.0f, 360.0f),
            true,                    // vertical row
            21,                        // 10 circles
            19.0,                     // zindex
            -65.0f,                   // spacing
            15.0f,                    // radius
            Util::Color(0.0, 1.0, 1.0, 0.4),
            glm::vec2(0.0f, -1.0f),// Move left
            110.0f,                    // Speed
            1200.0f,                   // Distance
            1.0f,                      // delay
            3.0f + wave * 3.8f,        // Start at
            0.0f                       // No interval (simultaneous)
        );
    }

    for (int wave = 0; wave < 4; wave++) {
        AddCrossLaserAttack(
             pattern,                       // 攻擊模式
             glm::vec2(-300.0f, -100.0f),   // 中心位置
             150.0f,                         // 寬度
             2500.0f,                       // 長度
             Util::Color(1.0, 0.8, 0.0, 0.6), // 黃色
             2.0f,                          // 倒數時間
             2.0f + wave * 5.0,            // 開始時間
             1.0f                           // 持續時間
        );
        AddCrossLaserAttack(
            pattern,                       // 攻擊模式
            glm::vec2(300.0f, 100.0f),   // 中心位置
            150.0f,                         // 寬度
            2500.0f,                       // 長度
            Util::Color(1.0, 0.8, 0.0, 0.6), // 黃色
            2.0f,                          // 倒數時間
            4.0f + wave * 5.0f,            // 開始時間
            1.0f                           // 持續時間
        );
    }
    pattern->SetDuration(25.0f);
    return pattern;
}

std::shared_ptr<AttackPattern> AttackPatternFactory::CreateBattle6Pattern() {
    auto pattern = std::make_shared<AttackPattern>();
    glm::vec2 centerPosition(0.0f, 0.0f);
    pattern->AddEnemyMovement([centerPosition](const std::shared_ptr<Enemy>& enemy, float totalTime) {
        enemy->MoveToPosition(centerPosition, totalTime);
    }, 0.0f, 1.0f);

    for (int wave = 0; wave < 10; wave++) {
        AddCircleAttackRow(
            pattern,                   // Pattern to add to
            glm::vec2(620.0f, 360.0f), // top right
            false,                    // vertical row
            7,                        // 10 circles
            20.0,                     // zindex
            -120.0f,                   // spacing
            20.0f,                    // radius
            Util::Color(0.0, 1.0, 1.0, 0.4),
            glm::vec2(-1.0f, 0.0f),// Move left
            300.0f,                    // Speed
            1200.0f,                   // Distance
            1.0f,                      // delay
            1.5f + wave * 1.0f,        // Start at
            0.0f                       // No interval (simultaneous)
        );
        AddCircleAttackRow(
            pattern,                   // Pattern to add to
            glm::vec2(-620.0f, 360.0f), // top left
            true,                    // vertical row
            10,                        // 10 circles
            20.0,                     // zindex
            128.0f,                   // spacing
            20.0f,                    // radius
            Util::Color(0.0, 1.0, 1.0, 0.4),
            glm::vec2(0.0f, -1.0f),// Move left
            300.0f,                    // Speed
            1200.0f,                   // Distance
            1.0f,                      // delay
            1.5f + wave * 0.8f,        // Start at
            0.0f                       // No interval (simultaneous)
        );
        AddCircleAttackRow(
            pattern,                   // Pattern to add to
            glm::vec2(-620.0f, 360.0f), // top right
            false,                    // vertical row
            7,                        // 10 circles
            20.0,                     // zindex
            -120.0f,                   // spacing
            20.0f,                    // radius
            Util::Color(0.0, 1.0, 1.0, 0.4),
            glm::vec2(1.0f, 0.0f),// Move left
            300.0f,                    // Speed
            1200.0f,                   // Distance
            1.0f,                      // delay
            11.0f + wave * 1.0f,        // Start at
            0.0f                       // No interval (simultaneous)
        );
        AddCircleAttackRow(
            pattern,                   // Pattern to add to
            glm::vec2(-620.0f, -360.0f), // top left
            true,                    // vertical row
            10,                        // 10 circles
            20.0,                     // zindex
            128.0f,                   // spacing
            20.0f,                    // radius
            Util::Color(0.0, 1.0, 1.0, 0.4),
            glm::vec2(0.0f, 1.0f),// Move left
            300.0f,                    // Speed
            1200.0f,                   // Distance
            1.0f,                      // delay
            11.0f + wave * 0.8f,        // Start at
            0.0f                       // No interval (simultaneous)
        );
    }

    for (int wave = 0; wave < 4; wave++) {
        AddCrossLaserAttack(
             pattern,                       // 攻擊模式
             glm::vec2(300.0f - wave * 600.0, 0.0f),   // 中心位置
             230.0f,                         // 寬度
             2500.0f,                       // 長度
             Util::Color(1.0, 0.8, 0.0, 0.6), // 黃色
             2.0f,                          // 倒數時間
             1.5f + wave * 6.0,            // 開始時間
             0.8f                           // 持續時間
        );
        AddCrossLaserAttack(
            pattern,                       // 攻擊模式
            glm::vec2(-300.0f, 0.0f),   // 中心位置
            230.0f,                         // 寬度
            2500.0f,                       // 長度
            Util::Color(1.0, 0.8, 0.0, 0.6), // 黃色
            2.0f,                          // 倒數時間
            1.5f + wave * 6.0f,                         // 開始時間
            0.8f                           // 持續時間
        );
    }
    std::vector positions = {glm::vec2(-580.0f, -300.0f), glm::vec2(-580.0f, 300.0f),
        glm::vec2(580.0f, 300.0f), glm::vec2(580.0f, -300.0f)};
    for (int wave = 0; wave < 8; wave++) {
        AddCrossLaserAttack(
             pattern,                       // 攻擊模式
             positions[wave%4],               // 中心位置
             230.0f,                         // 寬度
             2500.0f,                       // 長度
             Util::Color(0.0, 1.0, 1.0, 0.4), // 黃色
             2.0f,                          // 倒數時間
             2.0f + wave * 3.0f,                   // 開始時間
             0.8f                           // 持續時間
        );
    }

    pattern->SetDuration(26.0f);
    return pattern;
}

