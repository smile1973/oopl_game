#include "Attack/AttackPatternFactory.hpp"
#include "Util/Logger.hpp"
#include <cmath>
#include <random>

std::random_device rd;
std::mt19937 gen(rd());

AttackPatternFactory& AttackPatternFactory::GetInstance() {
    static AttackPatternFactory instance;
    return instance;
}

void AttackPatternFactory::AddCircleAttackRow(
    std::shared_ptr<AttackPattern> pattern,
    const glm::vec2& startPos,
    bool isHorizontal,
    int count,
    float zIndex,
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
        glm::vec2 position = startPos;
        if (isHorizontal) {
            position.x += static_cast<float>(i) * spacing;
        } else {
            position.y += static_cast<float>(i) * spacing;
        }

        auto attack = std::make_shared<CircleAttack>(position, delay, radius, i + 1);
        attack->SetColor(color);

        attack->SetAttackZIndex(zIndex);

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
    horizontalLaser->SetAutoRotation(false);
    horizontalLaser->SetAttackZIndex(10.0f);

    auto verticalLaser = std::make_shared<RectangleAttack>(
        centerPosition, delay, width, length, 1.57f, sequenceNumber + 1
    );
    verticalLaser->SetColor(color);
    verticalLaser->SetAttackDuration(duration);
    verticalLaser->SetAutoRotation(false);
    verticalLaser->SetAttackZIndex(10.0f);

    pattern->AddAttack(horizontalLaser, startTime);
    pattern->AddAttack(verticalLaser, startTime);
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

std::shared_ptr<AttackPattern> AttackPatternFactory::CreateBattle1Pattern() {
    auto pattern = std::make_shared<AttackPattern>();

    glm::vec2 centerPosition(200.0f, 0.0f);
    pattern->AddEnemyMovement([centerPosition](const std::shared_ptr<Enemy>& enemy, float totalTime) {
        enemy->MoveToPosition(centerPosition, totalTime);
    }, 0.0f, 1.5f);


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
            pattern,
            glm::vec2(-620.0f, 360.0f),
            true,
            10,
            30.0,
            128.0f,
            32.0f,
            Util::Color(0.0, 1.0, 0.3, 0.4),
            glm::vec2(0.0f, -1.0f),
            350.0f,
            720.0f,
            1.0f,
            1.0f + static_cast<float>(wave),// Start at 6 7 8
            0.0f
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
            pattern,
            glm::vec2(-620.0f, 360.0f),
            true,
            10,
            30.0,
            128.0f,
            32.0f,
            Util::Color(0.0, 1.0, 0.3, 0.4),
            glm::vec2(0.0f, -1.0f),
            350.0f,
            720.0f,
            1.0f,
            6.0f + static_cast<float>(wave),
            0.0f
        );
    }


    glm::vec2 upPosition(200.0f, 100.0f);
    pattern->AddEnemyMovement([upPosition](const std::shared_ptr<Enemy>& enemy, float totalTime) {
        enemy->MoveToPosition(upPosition, totalTime);
    }, 9.5f, 1.5f);

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
            pattern,
            glm::vec2(-620.0f, -360.0f),
            true,
            10,
            30.0f,
            128.0f,
            32.0f,
            Util::Color(0.0, 1.0, 0.3, 0.4),
            glm::vec2(0.0f, 1.0f),
            350.0f,
            720.0f,
            1.0f,
            12.0f + static_cast<float>(wave),
            0.0f
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
            pattern,
            glm::vec2(-620.0f, 360.0f),
            true,
            10,
            30.0f,
            128.0f,
            32.0f,
            Util::Color(0.0, 1.0, 0.3, 0.4),
            glm::vec2(0.0f, -1.0f),
            350.0f,
            720.0f,
            1.0f,
            17.0f + static_cast<float>(wave),
            0.0f
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
    }, 0.0f, 1.0f);

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
            pattern,
            glm::vec2(620.0f, 360.0f),
            false,
            11,
            20.0,
            -30.0f,
            20.0f,
            Util::Color(0.0, 1.0, 0.3, 0.4),
            glm::vec2(-1.0f, 0.0f),
            350.0f,
            1200.0f,
            1.0f,
            1.0f + static_cast<float>(wave) * 2.0,
            0.0f
        );
    }
    for (int wave = 0; wave < 3; wave++) {
        AddCircleAttackRow(
            pattern,
            glm::vec2(-620.0f, -360.0f),
            false,
            11,
            20.0,
            30.0f,
            20.0f,
            Util::Color(0.0, 1.0, 0.3, 0.4),
            glm::vec2(1.0f, 0.0f),
            350.0f,
            1200.0f,
            1.0f,
            2.0f + static_cast<float>(wave) * 2.0,
            0.0f
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
            pattern,
            pos1[count],
            200.0f,
            2500.0f,
            Util::Color(1.0, 0.8, 0.0, 0.6),
            2.0f,
            2.0f,
            1.0f
        );
        AddCrossLaserAttack(
            pattern,
            pos2[count],
            200.0f,
            2500.0f,
            Util::Color(1.0, 0.8, 0.0, 0.6),
            2.0f,
            7.0f,
            1.0f
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
            pattern,
            pos3[count],
            250.0f,
            2500.0f,
            Util::Color(1.0, 0.8, 0.0, 0.6),
            2.0f,
            12.5f + (static_cast<float>(count) * 0.3f),
            1.0f
        );
        AddCrossLaserAttack(
            pattern,
            pos4[count],
            250.0f,
            2500.0f,
            Util::Color(1.0, 0.8, 0.0, 0.6),
            2.0f,
            18.5f + (static_cast<float>(count) * 0.3f),
            1.0f
        );
    }
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
            pattern,
            glm::vec2(640.0f, 360.0f),
            true,
            20,
            20.0,
            -65.0f,
            15.0f,
            Util::Color(0.0, 1.0, 1.0, 0.4),
            glm::vec2(0.0f, -1.0f),
            110.0f,
            1200.0f,
            1.0f,
            1.0f + static_cast<float>(wave) * 3.8f,
            0.0f
        );
        AddCircleAttackRow(
            pattern,
            glm::vec2(685.0f, 360.0f),
            true,
            21,
            19.0,
            -65.0f,
            15.0f,
            Util::Color(0.0, 1.0, 1.0, 0.4),
            glm::vec2(0.0f, -1.0f),
            110.0f,
            1200.0f,
            1.0f,
            3.0f + static_cast<float>(wave) * 3.8f,
            0.0f
        );
    }

    for (int wave = 0; wave < 4; wave++) {
        AddCrossLaserAttack(
            pattern,
            glm::vec2(-300.0f, -100.0f),
            150.0f,
            2500.0f,
            Util::Color(1.0, 0.8, 0.0, 0.6),
            2.0f,
            2.0f + static_cast<float>(wave) * 5.0,
            1.0f
        );
        AddCrossLaserAttack(
            pattern,
            glm::vec2(300.0f, 100.0f),
            150.0f,
            2500.0f,
            Util::Color(1.0, 0.8, 0.0, 0.6),
            2.0f,
            4.0f + static_cast<float>(wave) * 5.0f,
            1.0f
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
            pattern,
            glm::vec2(620.0f, 360.0f),
            false,
            7,
            20.0,
            -120.0f,
            20.0f,
            Util::Color(0.0, 1.0, 1.0, 0.4),
            glm::vec2(-1.0f, 0.0f),
            300.0f,
            1200.0f,
            1.0f,
            1.5f + static_cast<float>(wave) * 1.0f,
            0.0f
        );
        AddCircleAttackRow(
            pattern,
            glm::vec2(-620.0f, 360.0f),
            true,
            10,
            20.0,
            128.0f,
            20.0f,
            Util::Color(0.0, 1.0, 1.0, 0.4),
            glm::vec2(0.0f, -1.0f),
            300.0f,
            1200.0f,
            1.0f,
            1.5f + static_cast<float>(wave) * 0.8f,
            0.0f
        );
        AddCircleAttackRow(
            pattern,
            glm::vec2(-620.0f, 360.0f),
            false,
            7,
            20.0,
            -120.0f,
            20.0f,
            Util::Color(0.0, 1.0, 1.0, 0.4),
            glm::vec2(1.0f, 0.0f),
            300.0f,
            1200.0f,
            1.0f,
            11.0f + static_cast<float>(wave) * 1.0f,
            0.0f
        );
        AddCircleAttackRow(
            pattern,
            glm::vec2(-620.0f, -360.0f),
            true,
            10,
            20.0,
            128.0f,
            20.0f,
            Util::Color(0.0, 1.0, 1.0, 0.4),
            glm::vec2(0.0f, 1.0f),
            300.0f,
            1200.0f,
            1.0f,
            11.0f + static_cast<float>(wave) * 0.8f,
            0.0f
        );
    }

    for (int wave = 0; wave < 4; wave++) {
        AddCrossLaserAttack(
            pattern,
            glm::vec2(300.0f - static_cast<float>(wave) * 600.0, 0.0f),
            230.0f,
            2500.0f,
            Util::Color(1.0, 0.8, 0.0, 0.6),
            2.0f,
            1.5f + static_cast<float>(wave) * 6.0,
            0.8f
        );
        AddCrossLaserAttack(
            pattern,
            glm::vec2(-300.0f, 0.0f),
            230.0f,
            2500.0f,
            Util::Color(1.0, 0.8, 0.0, 0.6),
            2.0f,
            1.5f + static_cast<float>(wave) * 6.0f,
            0.8f
        );
    }
    std::vector<glm::vec2> positions = {glm::vec2(-580.0f, -300.0f), glm::vec2(-580.0f, 300.0f),
        glm::vec2(580.0f, 300.0f), glm::vec2(580.0f, -300.0f)};
    for (int wave = 0; wave < 8; wave++) {
        AddCrossLaserAttack(
            pattern,
            positions[wave%4],
            230.0f,
            2500.0f,
            Util::Color(0.0, 1.0, 1.0, 0.4),
            2.0f,
            2.0f + static_cast<float>(wave) * 3.0f,
            0.8f
        );
    }

    pattern->SetDuration(26.0f);
    return pattern;
}

std::shared_ptr<AttackPattern> AttackPatternFactory::CreateBattle7Pattern() {
    auto pattern = std::make_shared<AttackPattern>();
    glm::vec2 centerPosition(0.0f, 0.0f);
    pattern->AddEnemyMovement([centerPosition](const std::shared_ptr<Enemy>& enemy, float totalTime) {
        enemy->MoveToPosition(centerPosition, totalTime);
    }, 0.0f, 1.0f);

    glm::vec2 startPos(340.0f, 0.0f);
    glm::vec2 endPos(-680.0f, 0.0f);
    for (int wave = 0; wave < 6; wave++) {
        auto attack = std::make_shared<CircleAttack>(startPos, 4.0f, 110.0f);
        attack->SetColor(Util::Color(1.0, 0.4, 0.4, 0.7));
        attack->SetMovementParams(glm::normalize(endPos - startPos), 600.0f, glm::length(endPos - startPos));
        pattern->AddAttack(attack, 1.0f + static_cast<float>(wave) * 4.0f);
    }
    startPos = {-340.0f, 0.0f};
    endPos = {680.0f, 0.0f};
    for (int wave = 0; wave < 6; wave++) {
        auto attack = std::make_shared<CircleAttack>(startPos, 4.0f, 110.0f);
        attack->SetColor(Util::Color(1.0, 0.4, 0.4, 0.7));
        attack->SetMovementParams(glm::normalize(endPos - startPos), 600.0f, glm::length(endPos - startPos));
        pattern->AddAttack(attack, 1.0f + static_cast<float>(wave) * 4.0f);
    }
    std::vector<glm::vec2> up = {{680.0f, 180.0f}, {-340.0f, 180.0f}, {-680.0f, 180.0f}, {340.0f, 180.0f}};
    std::vector<glm::vec2> down = {{680.0f, -180.0f}, {-340.0f, -180.0f}, {-680.0f, -180.0f}, {340.0f, -180.0f}};
    for (int wave = 0; wave < 6; wave++) {
        if (wave == 0 || wave == 2 || wave == 3) {
            auto attacka = std::make_shared<CircleAttack>(up[3], 4.0f, 110.0f);
            auto attackb = std::make_shared<CircleAttack>(up[1], 4.0f, 110.0f);
            attacka->SetColor(Util::Color(1.0, 0.4, 0.4, 0.7));
            attacka->SetMovementParams(glm::normalize(up[2] - up[3]), 600.0f, glm::length(up[2] - up[3]));
            attackb->SetColor(Util::Color(1.0, 0.4, 0.4, 0.7));
            attackb->SetMovementParams(glm::normalize(up[0] - up[1]), 600.0f, glm::length(up[2] - up[3]));
            pattern->AddAttack(attacka, 1.0f + static_cast<float>(wave) * 4.0f);
            pattern->AddAttack(attackb, 1.0f + static_cast<float>(wave) * 4.0f);
        }else {
            auto attacka = std::make_shared<CircleAttack>(down[3], 4.0f, 110.0f);
            auto attackb = std::make_shared<CircleAttack>(down[1], 4.0f, 110.0f);
            attacka->SetColor(Util::Color(1.0, 0.4, 0.4, 0.7));
            attacka->SetMovementParams(glm::normalize(down[2] - down[3]), 600.0f, glm::length(down[2] - down[3]));
            attackb->SetColor(Util::Color(1.0, 0.4, 0.4, 0.7));
            attackb->SetMovementParams(glm::normalize(down[0] - down[1]), 600.0f, glm::length(down[2] - down[3]));
            pattern->AddAttack(attacka, 1.0f + static_cast<float>(wave) * 4.0f);
            pattern->AddAttack(attackb, 1.0f + static_cast<float>(wave) * 4.0f);
        }
    }
    startPos = {250.0f, 400.0f};
    endPos = {250.0f, -400.0f};
    for (int wave = 0; wave < 3; wave++) {
        auto attack = std::make_shared<CircleAttack>(startPos, 3.0f, 280.0f);
        attack->SetColor(Util::Color(1.0, 0.4, 0.4, 0.7));
        attack->SetMovementParams(glm::normalize(endPos - startPos), 200.0f, glm::length(endPos - startPos));
        pattern->AddAttack(attack, 1.0f + static_cast<float>(wave) * 8.0f);
    }
    startPos = {-250.0f, 400.0f};
    endPos = {-250.0f, -400.0f};
    for (int wave = 0; wave < 3; wave++) {
        auto attack = std::make_shared<CircleAttack>(startPos, 3.0f, 280.0f);
        attack->SetColor(Util::Color(1.0, 0.4, 0.4, 0.7));
        attack->SetMovementParams(glm::normalize(endPos - startPos), 200.0f, glm::length(endPos - startPos));
        pattern->AddAttack(attack, 5.0f + static_cast<float>(wave) * 8.0f);
    }
    pattern->SetDuration(21.0f);
    return pattern;
}

std::shared_ptr<AttackPattern> AttackPatternFactory::CreateBattle8Pattern() {
    auto pattern = std::make_shared<AttackPattern>();
    glm::vec2 centerPosition(0.0f, 0.0f);
    pattern->AddEnemyMovement([centerPosition](const std::shared_ptr<Enemy>& enemy, float totalTime) {
        enemy->MoveToPosition(centerPosition, totalTime);
    }, 0.0f, 1.0f);

    float duration = 33.0f;
    std::vector<glm::vec2> pos = {{-500.0, 0.0}, {500.0, 0.0}};
    auto laserattack1 = std::make_shared<RectangleAttack>(
        pos[0], 2.0, 500.0f, 720.0f, 0.0f, 1
    );
    laserattack1->SetAutoRotation(false);
    laserattack1->SetAttackDuration(duration);
    auto laserattack2 = std::make_shared<RectangleAttack>(
        pos[1], 2.0, 500.0f, 720.0f, 0.0f, 1
    );
    laserattack2->SetAutoRotation(false);
    laserattack2->SetAttackDuration(duration);
    pattern->AddAttack(laserattack1, 1.0f);
    pattern->AddAttack(laserattack2, 1.0f);

    auto rotateAttack1 = std::make_shared<RectangleAttack>(
        centerPosition, 2.0, 1500.0f, 20.0f, 0.0f, 1
    );
    float rotationSpeed = 0.1f;
    rotateAttack1->SetAutoRotation(true, rotationSpeed);
    rotateAttack1->SetAttackDuration(30.0f);
    rotateAttack1->SetAttackZIndex(30.0f);
    pattern->AddAttack(rotateAttack1, 1.0f);

    std::uniform_real_distribution<float> y_pos(-360, 360);
    std::uniform_real_distribution<float> x_pos(-125, 125);
    for (int wave = 0; wave < 12; wave++) {
        auto laser = std::make_shared<RectangleAttack>(
            glm::vec2{x_pos(gen), y_pos(gen)}, 1.0f, 250.0f, 250.0, 0.0, 1+wave
        );
        laser->SetAutoRotation(false);
        laser->SetAttackDuration(0.5f);
        pattern->AddAttack(laser, 4.0f + wave * 1.0);
    }

    std::uniform_int_distribution<int> range_selector(0, 2);
    int selected_range = range_selector(gen);

    std::uniform_real_distribution<float> rot;
    if (selected_range == 0) {
        rot = std::uniform_real_distribution<float>(-1.2f, 1.2f);
    } else if (selected_range == 1) {
        rot = std::uniform_real_distribution<float>(1.8f, 3.14f);
    } else {
        rot = std::uniform_real_distribution<float>(-3.14f, -1.8f);
    }
    for (int wave = 0; wave < 8; wave++) {
        auto laser = std::make_shared<RectangleAttack>(
            glm::vec2{x_pos(gen), y_pos(gen)}, 1.0f, 1000.0f, 150.0, rot(gen), 1+wave
        );
        laser->SetAutoRotation(false);
        laser->SetAttackDuration(0.5f);
        pattern->AddAttack(laser, 18.0f + wave * 1.5);
    }

    pattern->SetDuration(30.0f);
    return pattern;
}

std::shared_ptr<AttackPattern> AttackPatternFactory::BossPattern1() {
    auto pattern = std::make_shared<AttackPattern>();
    glm::vec2 centerPosition(0.0f, 0.0f);
    pattern->AddEnemyMovement([centerPosition](const std::shared_ptr<Enemy>& enemy, float totalTime) {
        enemy->MoveToPosition(centerPosition, totalTime);
    }, 0.0f, 1.0f);

    int count = 16;
    std::vector<glm::vec2> positions = CalculateCircularPositions(centerPosition, 300, count);
    for (int wave = 0; wave < count; wave++) {
        auto laser = std::make_shared<RectangleAttack>(
            positions[wave], 0.5, 2000, 80, (-2.0 * M_PI/count) * wave, 1+wave
        );
        laser->SetAutoRotation(false);
        laser->SetAttackDuration(0.4f);
        pattern->AddAttack(laser, 1.0f + wave * 0.3);
    }
    pattern->SetDuration(2.0f + count * 0.4f);
    return pattern;
}
std::shared_ptr<AttackPattern> AttackPatternFactory::BossPattern2() {
    auto pattern = std::make_shared<AttackPattern>();
    glm::vec2 centerPosition(0.0f, 0.0f);
    pattern->AddEnemyMovement([centerPosition](const std::shared_ptr<Enemy>& enemy, float totalTime) {
        enemy->MoveToPosition(centerPosition, totalTime);
    }, 0.0f, 1.0f);

    int count = 6;
    float delay = 0.4f;
    float interval = 0.5f;
    float duration = 0.8f;
    for (int wave = 0; wave < count; wave++) {
        auto laser = std::make_shared<RectangleAttack>(
            glm::vec2{100.0, -320 + (wave * (720 / count))}, delay, 2000, 50, -0.5, 1+wave
        );
        laser->SetAutoRotation(false);
        laser->SetAttackDuration(duration);
        pattern->AddAttack(laser, 1.0f + wave * interval);
    }
    for (int wave = 0; wave < count; wave++) {
        auto laser = std::make_shared<RectangleAttack>(
            glm::vec2{-100.0, 320 - (wave * (720 / count))}, delay, 2000, 50, 0.5, 1+wave
        );
        laser->SetAutoRotation(false);
        laser->SetAttackDuration(duration);
        pattern->AddAttack(laser, 1.25f + (wave * interval));
    }
    pattern->SetDuration(2.0f + 2 * count * interval);
    return pattern;
}
std::shared_ptr<AttackPattern> AttackPatternFactory::BossPattern3() {
    auto pattern = std::make_shared<AttackPattern>();
    glm::vec2 centerPosition(0.0f, 0.0f);
    pattern->AddEnemyMovement([centerPosition](const std::shared_ptr<Enemy>& enemy, float totalTime) {
        enemy->MoveToPosition(centerPosition, totalTime);
    }, 0.0f, 1.0f);

    int count = 8;
    std::vector<glm::vec2> positions = CalculateCircularPositions(centerPosition, 500, count);
    float delay = 2.0f;
    float interval = 1.5f;
    float duration = 0.5f;
    std::uniform_real_distribution<float> rot(0.6, 1.1);
    for (int wave = 0; wave < count; wave++) {
        auto laser = std::make_shared<RectangleAttack>(
            positions[2 * (wave % 4) + 1], delay, 2000.0f, 900.0f + (wave / 4) * 100, pow(-1, wave) * rot(gen) , 1+wave
        );
        laser->SetAutoRotation(false);
        laser->SetAttackDuration(duration);
        pattern->AddAttack(laser, 1.0f + wave * interval);
    }
    pattern->SetDuration(2.0f + count * interval);
    return pattern;
}
std::shared_ptr<AttackPattern> AttackPatternFactory::BossPattern4() {
    auto pattern = std::make_shared<AttackPattern>();
    glm::vec2 centerPosition(0.0f, 0.0f);
    pattern->AddEnemyMovement([centerPosition](const std::shared_ptr<Enemy>& enemy, float totalTime) {
        enemy->MoveToPosition(centerPosition, totalTime);
    }, 0.0f, 1.0f);

    for (int wave = 0; wave < 4; wave++) {
        AddCircleAttackRow(
            pattern,
            glm::vec2(620.0f, 360.0f),
            false,
            10,
            30.0,
            -36.0f,
            35.0f,
            Util::Color(0.0, 1.0, 1.0, 0.4),
            glm::vec2(-1.0f, 0.0f),
            300.0f,
            1500.0f,
            0.5f,
            1.5f + static_cast<float>(wave) * 2.0f,
            0.0f
        );
        AddCircleAttackRow(
            pattern,
            glm::vec2(-620.0f, 0.0f),
            false,
            10,
            30.0,
            -36.0f,
            35.0f,
            Util::Color(0.0, 1.0, 1.0, 0.4),
            glm::vec2(1.0f, 0.0f),
            300.0f,
            1500.0f,
            0.5f,
            1.5f + static_cast<float>(wave) * 2.3f,
            0.0f
        );
    }

    float delay = 3.0f;
    auto laser11 = std::make_shared<RectangleAttack>(
        glm::vec2{0.0, -180.0}, delay, 2000.0f, 360, 0
    );
    auto laser12 = std::make_shared<RectangleAttack>(
        glm::vec2{400.0, 0.0}, delay, 1200.0f, 1000.0f, 0
    );
    pattern->AddAttack(laser11, 7.0f);
    pattern->AddAttack(laser12, 7.0f);

    auto laser21 = std::make_shared<RectangleAttack>(
        glm::vec2{0.0, 180.0}, delay, 2000.0f, 360, 0
    );
    auto laser22 = std::make_shared<RectangleAttack>(
        glm::vec2{-330, 0.0}, delay, 660.0f, 1000.0f, 0
    );
    pattern->AddAttack(laser21, 1.0f);
    pattern->AddAttack(laser22, 1.0f);
    pattern->SetDuration(10.0f);
    return pattern;
}
