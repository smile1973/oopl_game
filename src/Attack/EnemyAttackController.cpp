#include "Attack/EnemyAttackController.hpp"
#include "Attack/AttackManager.hpp"
#include "Util/Logger.hpp"
#include "Util/Time.hpp"

EnemyAttackController::EnemyAttackController(std::shared_ptr<Enemy> enemy)
    : m_Enemy(enemy) {
    std::random_device rd;
    m_RandomEngine.seed(rd());
}

void EnemyAttackController::InitBattle1Patterns() {
    ClearPatterns();
    auto pattern = AttackPatternFactory::GetInstance().CreateBattle1Pattern();
    AddPattern(pattern);
}

void EnemyAttackController::InitBattle2Patterns() {
    ClearPatterns();
    auto pattern = AttackPatternFactory::GetInstance().CreateBattle2Pattern();
    AddPattern(pattern);
}

void EnemyAttackController::InitBattle3Patterns() {
    ClearPatterns();
    auto pattern = AttackPatternFactory::GetInstance().CreateBattle3Pattern();
    AddPattern(pattern);
}

void EnemyAttackController::InitBattle4Patterns() {
    ClearPatterns();
    auto pattern = AttackPatternFactory::GetInstance().CreateBattle4Pattern();
    AddPattern(pattern);
}

void EnemyAttackController::InitBattle5Patterns() {
    ClearPatterns();
    auto pattern = AttackPatternFactory::GetInstance().CreateBattle5Pattern();
    AddPattern(pattern);
}

void EnemyAttackController::InitBattle6Patterns() {
    ClearPatterns();
    auto pattern = AttackPatternFactory::GetInstance().CreateBattle6Pattern();
    AddPattern(pattern);
}

void EnemyAttackController::InitBattle7Patterns() {
    ClearPatterns();
    auto pattern = AttackPatternFactory::GetInstance().CreateBattle7Pattern();
    AddPattern(pattern);
}

void EnemyAttackController::InitBattle8Patterns() {
    ClearPatterns();
    auto pattern = AttackPatternFactory::GetInstance().CreateBattle8Pattern();
    AddPattern(pattern);
}

void EnemyAttackController::InitBossPatterns() {
    m_BossPatternTypes.clear();
    m_BossPatternTypes = {0, 1, 2, 3};
    SelectRandomPatternForBoss();
}

void EnemyAttackController::SelectRandomPatternForBoss() {
    if (m_BossPatternTypes.empty()) return;

    std::uniform_int_distribution<int> distribution(0, m_BossPatternTypes.size() - 1);
    int randomIndex = distribution(m_RandomEngine);

    int patternType = m_BossPatternTypes[randomIndex];

    ClearPatterns();
    std::shared_ptr<AttackPattern> newPattern = nullptr;

    switch (patternType) {
        case 0:
            newPattern = AttackPatternFactory::GetInstance().BossPattern1();
        break;
        case 1:
            newPattern = AttackPatternFactory::GetInstance().BossPattern2();
        break;
        case 2:
            newPattern = AttackPatternFactory::GetInstance().BossPattern3();
        break;
        case 3:
            newPattern = AttackPatternFactory::GetInstance().BossPattern4();
        break;
        default:
            newPattern = AttackPatternFactory::GetInstance().CreateBattle1Pattern();
        break;
    }

    if (newPattern) {
        AddPattern(newPattern);
        // LOG_DEBUG("Created new pattern instance of type {} for Boss", patternType);
    }
}

void EnemyAttackController::Update(float deltaTime, std::shared_ptr<Character> player) {
    if (!m_IsActive) return;

    if (m_IsInCooldown) {
        m_ElapsedCooldownTime += deltaTime;

        if (m_ElapsedCooldownTime >= m_CooldownTime) {
            // 冷卻結束，切換到下一個模式
            m_IsInCooldown = false;
            m_ElapsedCooldownTime = 0.0f;
            SwitchToNextPattern();
        }
        return;
    }

    // 更新當前攻擊模式
    if (m_CurrentPattern) {
        m_CurrentPattern->Update(deltaTime, player);
        if (m_CurrentPattern->IsFinished()) {
            m_IsInCooldown = true;
            m_ElapsedCooldownTime = 0.0f;
        }
    } else {
        SwitchToNextPattern();
        if (!m_CurrentPattern && m_PatternQueue.empty() && !m_IsInCooldown) {
            if (m_CurrentMainPhase == 3 && m_CurrentSubPhase == 4) SelectRandomPatternForBoss();
            else InitPatternsForCurrentPhase();
        }
    }
}

bool EnemyAttackController::IsAllPatternsCompleted() const {
    return !m_CurrentPattern && m_PatternQueue.empty() && !m_IsInCooldown;
}

void EnemyAttackController::AddPattern(std::shared_ptr<AttackPattern> pattern) {
    if (pattern) {
        m_PatternQueue.push(pattern);
    }
}

void EnemyAttackController::ClearPatterns() {
    while (!m_PatternQueue.empty()) {
        m_PatternQueue.pop();
    }

    m_CurrentPattern = nullptr;
    m_IsInCooldown = false;
    m_ElapsedCooldownTime = 0.0f;
}

void EnemyAttackController::Start() {
    m_IsActive = true;

    // 如果有當前模式，直接啟動
    if (m_CurrentPattern) {
        m_CurrentPattern->Start(m_Enemy);
    } else {
        // 嘗試切換到下一個模式
        SwitchToNextPattern();
    }
}

void EnemyAttackController::Stop() {
    m_IsActive = false;
    if (m_CurrentPattern) {
        m_CurrentPattern->Stop();
    }
}

void EnemyAttackController::Reset() {
    Stop();
    ClearPatterns();
}

void EnemyAttackController::SwitchToNextPattern() {
    // 如果隊列為空，沒有更多模式
    if (m_PatternQueue.empty()) {
        m_CurrentPattern = nullptr;
        return;
    }

    // 獲取下一個模式
    m_CurrentPattern = m_PatternQueue.front();
    m_PatternQueue.pop();

    // 開始新模式
    if (m_CurrentPattern && m_IsActive) {
        m_CurrentPattern->Start(m_Enemy);
    }
}

void EnemyAttackController::InitPatternsForCurrentPhase() {
    // 清空現有的模式
    ClearPatterns();

    if (m_CurrentMainPhase == 3 && m_CurrentSubPhase == 4) {
        InitBossPatterns();
        Start();
        return;
    }

    if (m_CurrentMainPhase == 1) { // 第一大關
        if (m_CurrentSubPhase == 1) InitBattle1Patterns();      // 敵人1
        if (m_CurrentSubPhase == 2) InitBattle2Patterns(); // 敵人2
        else if (m_CurrentSubPhase == 4) InitBattle3Patterns(); // 敵人3
    }
    else if (m_CurrentMainPhase == 2) { // 第二大關
        if (m_CurrentSubPhase == 1) InitBattle4Patterns();      // 敵人4
        else if (m_CurrentSubPhase == 2) InitBattle5Patterns(); // 敵人5
        else if (m_CurrentSubPhase == 4) InitBattle6Patterns(); // 敵人6
    }
    else if (m_CurrentMainPhase == 3) { // 第三大關
        if (m_CurrentSubPhase == 1) InitBattle7Patterns();      // 敵人7
        else if (m_CurrentSubPhase == 2) InitBattle8Patterns(); // 敵人8
    }
    Start();
}