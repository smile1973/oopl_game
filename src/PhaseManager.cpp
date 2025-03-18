#include "PhaseManger.hpp"

#include "Util/Logger.hpp"

PhaseManager::PhaseManager() {
    m_Background = std::make_shared<BackgroundImage>();
}

bool PhaseManager::NextSubPhase() {
    m_SubPhase++;
    LOG_DEBUG("Into SubPhase: {}-{}", m_MainPhase, m_SubPhase);

    // 如果完成了所有6個小關，返回true表示應該進入下一大關
    return m_SubPhase >= MAX_SUB_PHASES;
}

void PhaseManager::NextMainPhase() {
    m_MainPhase++;
    m_SubPhase = 0;

    using MainPhaseType = BackgroundImage::MainPhaseType;
    const auto phaseType = static_cast<MainPhaseType>(m_MainPhase);
    auto phaseName = "UNKNOWN";
    switch (phaseType) {
        case MainPhaseType::INITIAL_SCENE: phaseName = "INITIAL_SCENE"; break;
        case MainPhaseType::KINGDOM_OUTSKIRTS: phaseName = "KINGDOM_OUTSKIRTS"; break;
        case MainPhaseType::SCHOLARS_NEST: phaseName = "SCHOLARS_NEST"; break;
        case MainPhaseType::KINGS_ARSENAL: phaseName = "KINGS_ARSENAL"; break;
        case MainPhaseType::RED_DARKHOUSE: phaseName = "RED_DARKHOUSE"; break;
        case MainPhaseType::CHURCHMOUSE_STREETS: phaseName = "CHURCHMOUSE_STREETS"; break;
    }
    LOG_DEBUG("--{}--", phaseName);

    // 設置新的背景
    m_Background->SetMainPhase(phaseType);
}
