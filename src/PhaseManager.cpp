#include <iostream>
#include <ostream>

#include "PhaseManger.hpp"

#include "Util/Logger.hpp"

/**
 * @brief 進入下一個小關。
 */
void PhaseManager::NextSubPhase() {
    if (m_MainPhase == 0) {
        NextMainPhase();
        return;
    }
    m_SubPhase++;
    LOG_DEBUG("Into SubPhase: {}-{}--{}", m_MainPhase, m_SubPhase, GetSubPhaseName(m_SubPhase));

    if (m_SubPhase > m_MaxSubPhase) {
        m_SubPhase = 0;
        NextMainPhase();
    }
}

/**
 * @brief 更新小關類型。
 */
void PhaseManager::UpdateSubPhaseType() {
    UpdateStageTitle();
    switch (m_SubPhase) {
        case 0: m_SubPhaseType = 0; //STORE
            break;
        case 1:
        case 2:
        case 3: m_SubPhaseType = 1; //BATTLE
            break;
        case 4: m_SubPhaseType = 2; //TREASURE
            break;
        case 5: m_SubPhaseType = 3; //BOSS
            break;
        default: LOG_ERROR("Invalid SubPhaseType: {}", m_SubPhase);
    }
}

/**
 * @brief 進入下一個大關。
 */
void PhaseManager::NextMainPhase() {
    m_MainPhase++;
    m_SubPhase = 0;

    if (m_MainPhase > m_MaxMainPhase) {
        m_MainPhase = 0;
        std::cout << "\nClear All Phase\n" << std::endl;
    }
    LOG_INFO("Into--{}--", GetMainPhaseName(m_MainPhase));

    // 設置新的背景
    m_Background->SetBackground(m_MainPhase);

    UpdateSubPhaseType();
}

void PhaseManager::UpdateStageTitle() const {
    m_MainStageTitle->SetStageTitle(m_MainPhase-1);
    m_MainStageTitle->MovePosition(glm::vec2(100, 0));
    m_MainStageTitle->SetVisible(true);
    m_MainStageTitle->MovePosition(glm::vec2(-100, 0), 1.5f);
}


void PhaseManager::Update() const {
    m_MainStageTitle->Update();
    if (m_MainStageTitle->GetPosition() == m_MainStageTitle->GetTargetPosition() ) m_MainStageTitle->SetVisible(false);
}
