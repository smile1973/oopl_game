#include "DefeatScreen.hpp"
#include "Util/Logger.hpp"
#include "Util/Time.hpp"

#include <iomanip>


DefeatScreen::DefeatScreen(const std::shared_ptr<Character>& character)
    : m_Character(character) {

    m_BaseObject = std::make_shared<Object>(GA_RESOURCE_DIR "/Image/Background/defeat_screen.png");
    m_BaseObject -> SetPosition(glm::vec2(0, 0));
    m_BaseObject -> SetScale(0.75,0.75);
    m_BaseObject -> SetZIndex(90);

    m_GameTime = std::make_shared<TextObject>(StringGameTime(), 27);
    m_GameTime -> SetPosition(glm::vec2(15, 302));
    m_GameTime -> SetZIndex(100);

    m_Level = std::make_shared<TextObject>("11", 20);
    m_Level -> SetPosition(glm::vec2(85, 142));
    m_Level -> SetZIndex(100);

    m_UserName = std::make_shared<TextObject>("UserName", 20);
    m_UserName -> SetPosition(glm::vec2(-60, 142));
    m_UserName -> SetZIndex(100);

    m_DPS = std::make_shared<TextObject>("123", 20);
    m_DPS -> SetPosition(glm::vec2(65, -175));
    m_DPS -> SetZIndex(100);

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


    for (const auto& option : m_Options) {
        m_Children.push_back(option);
    }
    m_Children.push_back(m_BaseObject);
    m_Children.push_back(m_GameTime);
    m_Children.push_back(m_UserName);
    m_Children.push_back(m_Level);
    m_Children.push_back(m_DPS);
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
    LOG_INFO("DefeatScreen::Reset");
}

void DefeatScreen::Get() {
    m_CurrentOption = -1;
    m_IsGameStart = false;
    SetVisible(true);
    m_GameTime -> SetText(StringGameTime(m_GameTimer));
    // m_GameTime -> SetText("StringGameTime(m_GameTimer)");
}

void DefeatScreen::Update(){
    if (m_IsGameStart) {
        m_GameTimer += Util::Time::GetDeltaTimeMs();
        // LOG_INFO("DefeatScreen::Update--m_GameTimer:{}",m_GameTimer);
    }
    for (const auto& option : m_Options) {
        option->Update();
    }
}

