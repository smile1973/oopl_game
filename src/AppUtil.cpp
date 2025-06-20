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

        m_Enemy_dummy->SetHealth();
        m_Enemy_dummy->SetVisible(true);
        m_Enemy_dummy->MoveToPosition(glm::vec2(100,0),1.3);
        m_Onward->SetVisible(true);
        m_Onward->MoveToPosition(glm::vec2(500,160),1.3);
    }

    if (m_Rabbit->GetPosition().x == -100) m_IsReady = true;

    m_SkillUI->Update();
    m_LevelUI->Update();
    m_HealthBarUI->Update();

    m_Rabbit->Update();
    m_Enemy_dummy->Update();
    m_Onward->Update();
    m_Root.Update();
}

/**
 * @brief 暫停畫面。
 */
void App::Pause() {
    m_PRM->SetProgressBarVisible(true);
    if (m_EnterDown && !Util::Input::IsKeyPressed(Util::Keycode::N)) {
        switch (m_PausedOption->GetCurrentOption()) {
            case 0:
                m_PausedOption->SetVisible(false);
                m_PRM->SetProgressBarVisible(false);
                LOG_DEBUG("--App::Pause Continue--");
            break;
            case 1:
                LOG_DEBUG("--App::Pause New_Game--");
                m_PausedOption->SetVisible(false);
                m_PRM->SetProgressBarVisible(false);
                RestartGame();
            break;
            case 4:
                LOG_DEBUG("--App::Pause Leave_Game--");
                m_PausedOption->SetVisible(false);
                m_CurrentState = State::END;
            break;
            default:
                LOG_ERROR("--App::Pause Switch Default--");
        }
    }
    m_EnterDown = Util::Input::IsKeyPressed(Util::Keycode::N);

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
 * @brief 結算畫面。
 */
void App::Defeat(){
    if (m_EnterDown && !Util::Input::IsKeyPressed(Util::Keycode::N)) {
        switch (m_DefeatScreen->GetCurrentOption()) {
            case 0:
                LOG_DEBUG("--App::Defeat Leave_Game--");
                m_DefeatScreen->SetVisible(false);
                m_CurrentState = State::END;
            break;
            case 1:
                LOG_DEBUG("--App::Defeat New_Game--");
                RestartGame();
            break;
            default:
                LOG_ERROR("--App::Defeat Switch Default--");
        }
    }
    m_EnterDown = Util::Input::IsKeyPressed(Util::Keycode::N);

    if (m_LeftKeyDown && !Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        m_DefeatScreen->Switch(true);
    }
    m_LeftKeyDown = Util::Input::IsKeyPressed(Util::Keycode::LEFT);

    if (m_RightKeyDown && !Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
        m_DefeatScreen->Switch(false);
    }
    m_RightKeyDown = Util::Input::IsKeyPressed(Util::Keycode::RIGHT);

    m_DefeatScreen->Update();
    m_Root.Update();
}

/**
 * @brief 商店畫面。
 */
void App::Shop() {
    if (m_EnterDown && !Util::Input::IsKeyPressed(Util::Keycode::E)) {
        m_shopUI->SetVisible(false);
        m_PRM->SetProgressBarVisible(true);
    }
    m_EnterDown = Util::Input::IsKeyPressed(Util::Keycode::E);

    if (m_LeftKeyDown && !Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        m_shopUI->SwitchProduct(true);
    }
    m_LeftKeyDown = Util::Input::IsKeyPressed(Util::Keycode::LEFT);

    if (m_RightKeyDown && !Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
        m_shopUI->SwitchProduct(false);
    }
    m_RightKeyDown = Util::Input::IsKeyPressed(Util::Keycode::RIGHT);

    if (m_NKeyDown && !Util::Input::IsKeyPressed(Util::Keycode::N)) {
        if (m_Rabbit->GetMoney()>=5) {
            if (m_shopUI->GetProduct()==0 && m_HealthBarUI->GetHealthBar()!=3) {
                m_Rabbit->AddMoney(-5);
                m_HealthBarUI->FullHealthBar();
            }
            if (m_shopUI->GetProduct()==1) {
                m_Rabbit->AddMoney(-5);
                m_Rabbit->AddExperience(100);
            }
        }
    }
    m_NKeyDown = Util::Input::IsKeyPressed(Util::Keycode::N);

    m_HealthBarUI->Update();
    m_LevelUI->Update();
    m_Rabbit->Update();
    m_Root.Update();
}
/**
 * @brief 驗證當前任務狀態，並切換至適當的階段。
 */
void App::ValidTask() {
    // 關卡跳轉
    if (m_Onward->GetVisibility() && m_Rabbit->IfCollide(m_Onward, 80)) {
        m_PRM->LeaveSubPhase();
        if (m_PRM->IfProgressBarSet()) {
            LeavePhase();
        }
    }
    if (m_HealthBarUI->GetHealthBar()<=0) {
        m_DefeatScreen->Get(false);
    }
    if (m_Enemy_shopkeeper->GetVisibility()) {
        if (m_RKeyDown && !Util::Input::IsKeyPressed(Util::Keycode::R)) {
            m_shopUI->SetVisible(true);
            m_PRM->SetProgressBarVisible(false);
        }
        m_RKeyDown = Util::Input::IsKeyPressed(Util::Keycode::R);
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
        case 2:
        case 4:
            AttackManager::GetInstance().ClearAllAttacks(); // 清除所有攻擊

            LOG_DEBUG("--Battle is over--");
        break;
        case 3:
            LOG_DEBUG("--Treasure opened--");
        break;
        default: ;
    }
    m_Rabbit->ResetSkill();
    SetSubPhase();
}

/**
 * @brief 設定下一個小關。
 */
void App::SetSubPhase() const {
    if (m_PRM->GetCurrentMainPhase() == 0)m_DefeatScreen->Reset();
    m_PRM->NextSubPhase();
    int m_SubPhaseIndex = m_PRM->GetCurrentSubPhase();

    // 重置玩家位置
    m_Rabbit->MoveToPosition({-400.0f, 160.0f}, 0);
    m_Rabbit->m_Transform.scale.x = 0.5;

    // 根據小關索引設置固定的小關類型
    switch (m_SubPhaseIndex) {
        case 0:
            LOG_DEBUG("Next SubPhase: STORE");
            m_DefeatScreen->AddPassedLevel(m_PRM->GetCurrentMainPhase());
            SetupStorePhase();
        break;
        case 1:
            m_Enemy_shopkeeper->SetVisible(false);
        [[fallthrough]];
        case 2:
            LOG_DEBUG("Next SubPhase: BATTLE {}", m_SubPhaseIndex);
        SetupBattlePhase();
        break;
        case 3:
            LOG_DEBUG("Next SubPhase: TREASURE");
            SetupTreasurePhase();
        break;
        case 4:
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

    // 第一關血量為150，每個大關增加100血量，每個小關增加50血量
    const float baseHealth = 100.0f + 250.0f * (MainPhaseIndex - 1) * (MainPhaseIndex);
    m_Overlay->SetVisible(true);
    // 設置基本敵人
    m_Enemy->SetPosition({197.5f, -3.5f});
    m_Enemy->SetVisible(true);
    m_Enemy->SetHealth(baseHealth + 150.0f * (SubPhaseIndex - 1) + 50.0f * std::max(m_Rabbit->GetLevel()-10,0));

    m_Enemy->SwitchImageSetByIndex(MainPhaseIndex*100 + SubPhaseIndex);

    // 設置敵人攻擊控制器
    if (m_EnemyAttackController) {
        m_EnemyAttackController->Reset();
        m_EnemyAttackController->SetCurrentPhase(MainPhaseIndex, SubPhaseIndex);
        m_EnemyAttackController->InitPatternsForCurrentPhase();
    }

    LOG_INFO("Set battle level: MainPhaseIndex {}, SubPhaseIndex {}", MainPhaseIndex, SubPhaseIndex);
}

/**
* @brief 重新開始遊戲的實現
*/
void App::RestartGame() {
    LOG_INFO("Restarting game...");

    // 1. 重置遊戲狀態
    m_IsReady = false;

    // 2. 重置玩家角色
    m_Rabbit->SetVisible(false);
    m_Rabbit->Reset(); // 假設Character類中有此方法，如果沒有需要添加

    // 3. 重置敵人
    m_Enemy->Reset();
    m_Enemy_dummy->Reset();

    m_Enemy_shopkeeper->SetVisible(false);
    m_Enemy_treasure->SetVisible(false);

    // 4. 清除所有攻擊和特效
    AttackManager::GetInstance().ClearAllAttacks();
    Effect::EffectManager::GetInstance().ClearAllEffects(); // 假設有此方法，如果沒有需要添加

    // 5. 重置階段管理器
    m_PRM->ReStart(); // 已存在的方法，重置所有階段

    // 6. 重置UI元素
    m_GetReady->SetVisible(true);
    m_PressZtoJoin->SetVisible(true);
    m_Onward->SetPosition({500.0f, 160.0f});
    m_Onward->SetVisible(false);
    m_Overlay->SetVisible(false);
    m_HealthBarUI->Reset();
    m_HealthBarUI->SetVisible(false);
    m_LevelUI->SetVisible(false);

    // 7. 隱藏結算畫面
    m_DefeatScreen->SetVisible(false);
    m_DefeatScreen->Reset();

    // 8. 重置鍵盤狀態追蹤變量
    m_ZKeyDown = false;
    m_XKeyDown = false;
    m_CKeyDown = false;
    m_VKeyDown = false;
    m_NKeyDown = false;
    m_EnterDown = false;
    m_UpKeyDown = false;
    m_DownKeyDown = false;
    m_LeftKeyDown = false;
    m_RightKeyDown = false;

    LOG_INFO("Game restart completed. Waiting for player to press Z to join.");
}