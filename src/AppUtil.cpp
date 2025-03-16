#include "App.hpp"

#include "Util/Logger.hpp"
#include <random>

/**
 * @brief 驗證當前任務狀態，並切換至適當的階段。
 */
void App::ValidTask() {
    LOG_DEBUG("ValidTask: MainPhase {}, SubPhase {}",
              static_cast<int>(m_MainPhase),
              m_SubPhaseIndex);

    // 初始場景特殊處理
    if (m_MainPhase == MainPhase::INITIAL_SCENE) {
        LOG_DEBUG("--Initial_Scene--");
        NextMainPhase();
        return;
    }

    // 處理小關類型
    switch (m_CurrentSubPhase) {
        case SubPhase::BATTLE:
        case SubPhase::BOSS:
            if (m_Enemy->GetVisibility()) {
                LOG_DEBUG("The enemy is still alive");
                break;
            }

            LOG_DEBUG("--Battle is over--");
            NextSubPhase();
            break;

        case SubPhase::TREASURE:
            LOG_DEBUG("--Treasure opened--");
            NextSubPhase();
            break;

        case SubPhase::STORE:
            LOG_DEBUG("--Leave the store--");
            NextSubPhase();
            break;
        default: ;
    }
}

/**
 * @brief 進入下一個小關。
 */
void App::NextSubPhase() {
    // 使用PhaseManager進入下一小關，如果返回true表示需要進入下一大關
    if (m_PRM->NextSubPhase()) {
        NextMainPhase();
        return;
    }

    m_SubPhaseIndex++;

    // 根據小關索引設置固定的小關類型
    switch (m_SubPhaseIndex) {
        case 0:
            m_CurrentSubPhase = SubPhase::STORE;
            LOG_DEBUG("Next SubPhase: STORE");
            // 隱藏所有敵人
            m_Enemy->SetVisible(false);
            m_Enemy_bird_valedictorian->SetVisible(false);
            m_Enemy_dragon_silver->SetVisible(false);
            m_Rabbit->SetPosition({0.0f, 0.0f});
        break;
        case 1:
        case 2:
        case 3:
            m_CurrentSubPhase = SubPhase::BATTLE;
            LOG_DEBUG("Next SubPhase: BATTLE {}", m_SubPhaseIndex);
            SetupPhase();
        break;
        case 4:
            m_CurrentSubPhase = SubPhase::TREASURE;
            LOG_DEBUG("Next SubPhase: TREASURE");
            // 隱藏所有敵人
            m_Enemy->SetVisible(false);
            m_Enemy_bird_valedictorian->SetVisible(false);
            m_Enemy_dragon_silver->SetVisible(false);
            m_Rabbit->SetPosition({0.0f, 0.0f});
        break;
        case 5:
            m_CurrentSubPhase = SubPhase::BOSS;
            LOG_DEBUG("Next SubPhase: BOSS");
            SetupPhase();
        break;
        default:
            LOG_DEBUG("Wrong SubPhase Index: {}", m_SubPhaseIndex);
        break;
    }
}

/**
 * @brief 進入下一個大關。
 */
void App::NextMainPhase() {
    // 獲取當前大關索引
    int currentMainPhase = static_cast<int>(m_MainPhase);
    currentMainPhase++;

    // 如果所有大關都完成了，回到初始場景
    if (currentMainPhase > static_cast<int>(MainPhase::CHURCHMOUSE_STREETS)) {
        m_MainPhase = MainPhase::INITIAL_SCENE;
        LOG_DEBUG("All levels are completed and return to INITIAL_SCENE");
    } else {
        m_MainPhase = static_cast<MainPhase>(currentMainPhase);
        LOG_DEBUG("Into MainPhase: {}", currentMainPhase);
    }

    m_SubPhaseIndex = 0;
    m_CurrentSubPhase = SubPhase::STORE;

    // 切換背景
    m_PRM->NextMainPhase();

    // 設置初始戰鬥
    SetupPhase();
}

/**
 * @brief 設置關卡配置。
 */
void App::SetupPhase() {
    // 只有在戰鬥關卡才需要設置敵人
    if (m_CurrentSubPhase != SubPhase::BATTLE && m_CurrentSubPhase != SubPhase::BOSS) {
        return;
    }

    // 重置玩家位置
    m_Rabbit->SetPosition({-400.0f, 160.0f});

    // 根據大關和小關設置敵人難度
    int mainPhaseIndex = static_cast<int>(m_MainPhase);
    const float baseHealth = 50.0f * (mainPhaseIndex + 1);

    // 設置基本敵人
    m_Enemy->SetPosition({197.5f, -3.5f});
    m_Enemy->SetVisible(true);
    m_Enemy->SetHealth(baseHealth + 10.0f * m_SubPhaseIndex);

    // 根據小關索引增加敵人數量和難度
    if (m_SubPhaseIndex >= 1) {
        m_Enemy_bird_valedictorian->SetPosition({-197.5f, -103.5f});
        m_Enemy_bird_valedictorian->SetVisible(true);
        m_Enemy_bird_valedictorian->SetHealth(baseHealth + 8.0f * m_SubPhaseIndex);
    } else {
        m_Enemy_bird_valedictorian->SetVisible(false);
    }

    if (m_SubPhaseIndex >= 3) {
        m_Enemy_dragon_silver->SetPosition({0.0f, 100.0f});
        m_Enemy_dragon_silver->SetVisible(true);
        m_Enemy_dragon_silver->SetHealth(baseHealth * 1.2f + 12.0f * m_SubPhaseIndex);
    } else {
        m_Enemy_dragon_silver->SetVisible(false);
    }

    // 在每個大關的最後一個戰鬥關卡設置更強的敵人
    if (m_SubPhaseIndex == 4) {
        // 增加敵人生命值
        m_Enemy->SetHealth(baseHealth * 2.0f);
        m_Enemy_bird_valedictorian->SetHealth(baseHealth * 1.8f);
        m_Enemy_dragon_silver->SetHealth(baseHealth * 2.5f);

        // 顯示所有敵人
        m_Enemy_bird_valedictorian->SetVisible(true);
        m_Enemy_dragon_silver->SetVisible(true);

        LOG_DEBUG("The FINAL BATTLE level setting is completed");
    }

    LOG_DEBUG("Set battle level:MainPhaseIndex {}, SubPhaseIndex {}", mainPhaseIndex, m_SubPhaseIndex);
}
