#include "App.hpp"
#include "Attack/AttackManager.hpp"

#include "Util/Input.hpp"
#include "Util/Logger.hpp"
#include "Util/Keycode.hpp"

/**
 * @brief 初始準備階段。
 */
void App::GetReady() {
    // 按下Z
    if (m_ZKeyDown && m_Rabbit->GetVisibility()==false) {
        if (!Util::Input::IsKeyPressed(Util::Keycode::Z)) {
            m_PressZtoJoin->SetVisible(false);

            m_Rabbit->SetVisible(true);
            m_Rabbit->SetPosition(glm::vec2(-600,200));
            m_Rabbit->MoveToPosition(glm::vec2(-200,0),1.3);

            m_Enemy_dummy->SetPosition(glm::vec2(580,0));
            m_Onward->SetPosition(glm::vec2(980,160));
        }
    }
    m_ZKeyDown = Util::Input::IsKeyPressed(Util::Keycode::Z);

    if (m_Rabbit->GetPosition().x == -200) {
        m_Rabbit->MoveToPosition(glm::vec2(-100,0),1.3);

        m_Enemy_dummy->SetVisible(true);
        m_Enemy_dummy->MoveToPosition(glm::vec2(100,0),1.3);
        m_Onward->SetVisible(true);
        m_Onward->MoveToPosition(glm::vec2(500,160),1.3);
    }

    if (m_Rabbit->GetPosition().x == -100) m_IsReady = true;

    m_SkillUI->Update();
    m_Rabbit->Update();
    m_Enemy_dummy->Update();
    m_Onward->Update();
    m_Root.Update();
}

/**
 * @brief 暫停畫面。
 */
void App::Pause() {
    m_PausedOption->SetVisible(true);
    m_PRM->SetProgressBarVisible(true);
    if (m_EnterDown && !Util::Input::IsKeyPressed(Util::Keycode::M)) {
        switch (m_PausedOption->GetCurrentOption()) {
            case 0:
                m_PausedOption->SetVisible(false);
                m_PRM->SetProgressBarVisible(false);
                LOG_DEBUG("--App::Pause Continue--");
            break;
            default: ;
        }
        m_PausedOption->Reset();
    }
    m_EnterDown = Util::Input::IsKeyPressed(Util::Keycode::M);

    if (m_UpKeyDown && !Util::Input::IsKeyPressed(Util::Keycode::UP)) {
        m_PausedOption->Switch(true);
    }
    m_UpKeyDown = Util::Input::IsKeyPressed(Util::Keycode::UP);

    if (m_DownKeyDown && !Util::Input::IsKeyPressed(Util::Keycode::DOWN)) {
        m_PausedOption->Switch(false);
    }
    m_DownKeyDown = Util::Input::IsKeyPressed(Util::Keycode::DOWN);

    m_PRM->Update();
    m_PausedOption->Update();
    m_Root.Update();
}

/**
 * @brief 驗證當前任務狀態，並切換至適當的階段。
 */
void App::ValidTask() {
    // 關卡跳轉
    if (m_Onward->GetVisibility() && m_Rabbit->IfCollides(m_Onward, 80)) {
        m_PRM->LeaveSubPhase();
        if (m_PRM->IfProgressBarSet()) {
            LeavePhase();
        }
    }
}

/**
 * @brief 離開當前的階段。其實沒啥用
 */
void App::LeavePhase() const {
    int m_CurrentMainPhase = m_PRM->GetCurrentMainPhase();
    int m_CurrentSubPhase = m_PRM->GetCurrentSubPhase();
    int m_CurrentSubPhaseType = m_PRM->GetCurrentSubPhaseType();
    LOG_DEBUG("App::LeavePhase: MainPhase {}, SubPhase {}", m_CurrentMainPhase, m_CurrentSubPhase);
    // 處理小關類型
    switch (m_CurrentSubPhaseType) {
        case 0:
            LOG_DEBUG("--Leave the store--");
        break;
        case 1:
        case 3:
            AttackManager::GetInstance().ClearAllAttacks(); // 清除所有攻擊
            LOG_DEBUG("--Battle is over--");
        break;
        case 2:
            LOG_DEBUG("--Treasure opened--");
        break;
        default: ;
    }
    SetSubPhase();
}

/**
 * @brief 設定下一個小關。
 */
void App::SetSubPhase() const {
    m_PRM->NextSubPhase();
    int m_SubPhaseIndex = m_PRM->GetCurrentSubPhase();

    // 重置玩家位置
    m_Rabbit->MoveToPosition({-400.0f, 160.0f});

    // 根據小關索引設置固定的小關類型
    switch (m_SubPhaseIndex) {
        case 0:
            LOG_DEBUG("Next SubPhase: STORE");
            SetupStorePhase();
        break;
        case 1:
            m_Enemy_shopkeeper->SetVisible(false);
        [[fallthrough]];
        case 2:
        case 3:
            LOG_DEBUG("Next SubPhase: BATTLE {}", m_SubPhaseIndex);
        SetupBattlePhase();
        break;
        case 4:
            LOG_DEBUG("Next SubPhase: TREASURE");
            SetupTreasurePhase();
        break;
        case 5:
            LOG_DEBUG("Next SubPhase: BOSS");
            SetupBattlePhase();
        break;
        default:
            LOG_ERROR("Wrong SubPhase Index: {}", m_SubPhaseIndex);
        break;
    }
}

/**
 * @brief 設置商店關卡配置。
 */
void App::SetupStorePhase() const {
    LOG_INFO("Setup Phase: STORE");
    m_Enemy_shopkeeper->SetVisible(true);
    m_PRM->SetProgressBarVisible(true);
}
/**
 * @brief 設置寶箱關卡配置。
 */
void App::SetupTreasurePhase() const {
    LOG_INFO("Setup Phase: TREASURE");
    m_Enemy_treasure->SetVisible(true);
    m_Enemy_treasure->SetHealth();
}
/**
 * @brief 設置戰鬥關卡配置。
 */
void App::SetupBattlePhase() const {
    int MainPhaseIndex = m_PRM->GetCurrentMainPhase();
    int SubPhaseIndex = m_PRM->GetCurrentSubPhase();

    // 根據大關和小關設置敵人難度
    const float baseHealth = 50.0f * (MainPhaseIndex + 1);
    m_Overlay->SetVisible(true);
    // 設置基本敵人
    m_Enemy->SetPosition({197.5f, -3.5f});
    m_Enemy->SetVisible(true);
    m_Enemy->SetHealth(baseHealth + 10.0f * SubPhaseIndex);

    // 設置敵人攻擊控制器
    if (m_EnemyAttackController) {
        m_EnemyAttackController->Reset();
        m_EnemyAttackController->SetCurrentPhase(MainPhaseIndex, SubPhaseIndex);
        m_EnemyAttackController->InitPatternsForCurrentPhase();
    }

    LOG_DEBUG("Set battle level: MainPhaseIndex {}, SubPhaseIndex {}", MainPhaseIndex, SubPhaseIndex);
}