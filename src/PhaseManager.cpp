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

    UpdateSubPhaseType();
}

/**
 * @brief 更新小關類型。
 */
void PhaseManager::UpdateSubPhaseType() {
    switch (m_SubPhase) {
        case 0: m_SubPhaseType = 0; //STORE
        case 1:
        case 2:
        case 3: m_SubPhaseType = 1; //BATTLE
        case 4: m_SubPhaseType = 2; //TREASURE
        case 5: m_SubPhaseType = 3; //BOSS
            // break;
        default: LOG_ERROR("Invalid SubPhaseType: {}", m_SubPhase);
    }
     m_StageTitle->SetVisible(true);
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
}