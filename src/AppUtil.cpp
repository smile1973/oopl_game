#include "AppUtil.hpp"
#include "App.hpp"

#include "Util/Logger.hpp"

/**
 * @brief 驗證當前任務狀態，並切換至適當的階段。
 */
void App::ValidTask() {
    LOG_DEBUG("Validating task: {}", static_cast<int>(m_Phase));

    switch (m_Phase) {
        case Phase::START:
            LOG_DEBUG("--START--");
            m_Phase = Phase::BATTLE_1;
            m_Rabbit->SetPosition({-112.5f, -140.5f});
            m_Enemy->SetPosition({197.5f, -3.5f});
            m_Enemy->SetVisible(true);
            m_Enemy->SetHealth();
            m_PRM->NextPhase();
        break;
        case Phase::BATTLE_1:
            if (m_Enemy->IfAlive()) {
                LOG_DEBUG("The enemy is alive");
                break;
            }
            LOG_DEBUG("--BATTLE_1--");
            m_Phase = Phase::BATTLE_2;
            m_Rabbit->SetPosition({-112.5f, -140.5f});
            m_Enemy->SetPosition({197.5f, -3.5f});
            m_Enemy->SetVisible(true);
            m_Enemy->SetHealth();
            m_PRM->NextPhase();
        break;
        case Phase::BATTLE_2:
            if (m_Enemy->IfAlive()) {
                LOG_DEBUG("The enemy is alive");
                break;
            }
            LOG_DEBUG("--BATTLE_2--");
            m_Phase = Phase::BATTLE_3;
            m_Rabbit->SetPosition({-112.5f, -140.5f});
            m_Enemy->SetPosition({197.5f, -3.5f});
            m_Enemy->SetVisible(true);
            m_Enemy->SetHealth(30.0f);
            m_PRM->NextPhase();
        break;
        case Phase::BATTLE_3:
            if (m_Enemy->IfAlive()) {
                LOG_DEBUG("The enemy is alive");
                break;
            }
            LOG_DEBUG("--BATTLE_3--");
            m_Phase = Phase::BATTLE_4;
            m_Rabbit->SetPosition({-112.5f, -140.5f});
            m_Enemy->SetPosition({197.5f, -3.5f});
            m_Enemy->SetVisible(true);
            m_Enemy->SetHealth(200.0f);
            m_PRM->NextPhase();
        break;
        case Phase::BATTLE_4:
            if (m_Enemy->IfAlive()) {
                LOG_DEBUG("The enemy is alive");
                break;
            }
            LOG_DEBUG("--BATTLE_4--");
            m_Phase = Phase::STORE;
            m_Rabbit->SetPosition({-112.5f, -140.5f});
            m_Enemy->SetPosition({197.5f, -3.5f});
            m_Enemy->SetVisible(true);
            m_Enemy->SetHealth(1000.0f);
            m_PRM->NextPhase();
        break;

        case Phase::STORE:
            LOG_DEBUG("--STORE--");
        break;
    }
}