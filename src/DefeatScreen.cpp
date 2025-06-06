#include "DefeatScreen.hpp"
#include "Util/Logger.hpp"
#include "Util/Time.hpp"

#include <iomanip>


DefeatScreen::DefeatScreen(const std::shared_ptr<Character>& character)
    : m_Character(character) {

    m_BaseObject = std::make_shared<Object>(GA_RESOURCE_DIR "/Image/Background/defeat_screen.png");
    m_BaseObject -> SetPosition(glm::vec2(0, 0));
    m_BaseObject -> SetScale(0.75,0.75);
    m_BaseObject -> SetZIndex(85);

    m_GameTime = std::make_shared<TextObject>(StringGameTime(), 27);
    m_GameTime -> SetPosition(glm::vec2(15, 302));
    m_GameTime -> SetZIndex(100);

    m_Level = std::make_shared<TextObject>("11", 20);
    m_Level -> SetPosition(glm::vec2(85, 142));
    m_Level -> SetZIndex(100);

    m_UserName = std::make_shared<TextObject>("UserName", 20);
    m_UserName -> SetPosition(glm::vec2(-60, 142));
    m_UserName -> SetZIndex(100);

    const std::vector<std::string> optionNames = {"leave_game", "new_game"};
    for (int i = 0; i < optionNames.size(); ++i) {
        m_Options.push_back(std::make_shared<Object>(GA_RESOURCE_DIR "/Image/Option/" + optionNames[i] + ".png") );
        m_Options[i] -> SetScale(0.75,0.75);
        m_Options[i] -> SetZIndex(90);
    }
    constexpr int baseX = 260;
    constexpr int baseY = -276;
    m_Options[0] -> SetPosition(glm::vec2{-20-baseX, baseY});
    m_Options[1] -> SetPosition(glm::vec2{-40+baseX, baseY});

    const std::vector<std::string> PhasesNums = {"0", "1", "2"};
    constexpr int baseX2 = 30;
    constexpr int baseY2 = 230;
    constexpr float spacing = 126.0f;
    for (int i = 0; i < PhasesNums.size(); ++i) {
        m_PassedPhases.push_back(std::make_shared<Object>(GA_RESOURCE_DIR "/Image/UI/stage_icon_000" + PhasesNums[i] + ".png") );
        m_PassedPhases[i] -> SetScale(0.1,0.1);
        m_PassedPhases[i] -> SetZIndex(90);
        m_PassedPhases[i] -> SetPosition(glm::vec2{baseX2+spacing*i, baseY2});
        // LOG_ERROR("m_PassedPhases::{}", PhasesNums[i]);
    }


    for (const auto& option : m_Options) {
        m_Children.push_back(option);
    }
    for (const auto& option : m_PassedPhases) {
        m_Children.push_back(option);
    }
    m_Children.push_back(m_BaseObject);
    m_Children.push_back(m_GameTime);
    m_Children.push_back(m_UserName);
    m_Children.push_back(m_Level);
}


void DefeatScreen::SetVisible(const bool visible) {
    m_IfVisible = visible;
    for (const auto& child : m_Children) {
        child -> SetVisible(m_IfVisible);
    }
}

void DefeatScreen::Switch (const bool isLeft){
    if (m_CurrentOption != -1) {
        m_Options[m_CurrentOption]->m_Transform.scale =  {0.75f, 0.75f};
    }
    m_CurrentOption = isLeft ? (m_CurrentOption == 0) ? 1 : 0
                             : (m_CurrentOption == 1) ? 0 : 1;
    m_Options[m_CurrentOption]->m_Transform.scale =  {0.9f, 0.9f};
    LOG_INFO("Switch Current Option: " + std::to_string(m_CurrentOption));
}

void DefeatScreen::Reset() {
    m_CurrentOption = -1;
    m_GameTimer = 0;
    m_IsGameStart = true;
    for (const auto& option : m_Options) {
        option->m_Transform.scale =  {0.75f, 0.75f};
    }
    m_PassedLevels.clear();
    LOG_INFO("DefeatScreen::Reset");
}

void DefeatScreen::Get(const bool isVictory = false) {
    SetScreen(isVictory);
    m_CurrentOption = -1;
    m_IsGameStart = false;
    SetVisible(true);
    for (int i = m_PassedLevels.size(); i < m_PassedPhases.size(); ++i) {
        if (m_PassedPhases[i]) {
            m_PassedPhases[i]->SetVisible(false);
        }
    }
    m_GameTime -> SetText(StringGameTime(m_GameTimer));
    m_Level -> SetText(std::to_string(m_Character->GetLevel()));
}

void DefeatScreen::Update(){
    if (m_IsGameStart) {
        m_GameTimer += Util::Time::GetDeltaTimeMs();
    }
    for (const auto& option : m_Options) {
        option->Update();
    }
}

void DefeatScreen::SetScreen(const bool isVictory) const {
    if (isVictory) {
        m_BaseObject -> SetImage(GA_RESOURCE_DIR "/Image/Background/victory_screen.png");
    }else {
        m_BaseObject -> SetImage(GA_RESOURCE_DIR "/Image/Background/defeat_screen.png");
    }
}

void DefeatScreen::AddPassedLevel(const int mainPhase) {
    m_PassedLevels.push_back(mainPhase);
    // LOG_INFO("DefeatScreen::AddPassedLevel--PassedLevels:"+std::to_string(m_PassedLevels));

    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < m_PassedLevels.size(); ++i) {
        oss << m_PassedLevels[i];
        if (i < m_PassedLevels.size() - 1) {
            oss << ", ";
        }
    }
    oss << "]";
    LOG_INFO("DefeatScreen::AddPassedLevel--PassedLevels: {}", oss.str());
}

