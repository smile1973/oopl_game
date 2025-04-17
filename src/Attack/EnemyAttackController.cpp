#include "Attack/EnemyAttackController.hpp"
#include "Attack/AttackManager.hpp"
#include "Util/Logger.hpp"
#include "Util/Time.hpp"

EnemyAttackController::EnemyAttackController(std::shared_ptr<Enemy> enemy)
    : m_Enemy(enemy) {
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

void EnemyAttackController::Update(float deltaTime, std::shared_ptr<Character> player) {
    if (!m_IsActive) return;

    // 處理冷卻
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

        // 檢查當前模式是否已完成
        if (m_CurrentPattern->IsFinished()) {
            m_IsInCooldown = true;
            m_ElapsedCooldownTime = 0.0f;
        }
    } else {
        // 沒有當前模式，檢查隊列
        SwitchToNextPattern();

        if (!m_CurrentPattern && m_PatternQueue.empty() && !m_IsInCooldown) {
            // 自動重新初始化當前階段的攻擊模式
            InitPatternsForCurrentPhase();
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

    LOG_DEBUG("Enemy attack controller started");
}

void EnemyAttackController::Stop() {
    m_IsActive = false;

    // 停止當前模式
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
        LOG_DEBUG("Switched to next attack pattern");
    }
}

void EnemyAttackController::InitPatternsForCurrentPhase() {
    // 清空現有的模式
    ClearPatterns();

    // 根據主階段和子階段選擇攻擊模式
    if (m_CurrentMainPhase == 1 || m_CurrentMainPhase == 2) { // 第一大關
        if (m_CurrentSubPhase == 1) InitBattle1Patterns();
        if (m_CurrentSubPhase == 2) InitBattle2Patterns();
        if (m_CurrentSubPhase == 3) InitBattle3Patterns();
        if (m_CurrentSubPhase == 5) InitBattle2Patterns();
    } else { // 其他大關
        InitBattle1Patterns();
    }
    Start();
}