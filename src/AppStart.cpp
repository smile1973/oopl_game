#include "App.hpp"

#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");

    std::vector<std::string> rabbitImages;
    rabbitImages.reserve(2);
    for (int i = 0; i < 2; ++i) {
        rabbitImages.emplace_back(GA_RESOURCE_DIR"/Image/Character/hb_rabbit_idle" + std::to_string(i+1) + ".png");
    }

    std::vector<std::string> rabbitSkillImages;
    rabbitSkillImages.reserve(6);  // 假設有3張技能動畫圖片
    for (int i = 0; i < 6; ++i) {
        rabbitSkillImages.emplace_back(GA_RESOURCE_DIR"/Image/Character/hb_rabbit_skill1_" + std::to_string(i+1) + ".png");
    }

    m_Rabbit = std::make_shared<Character>(rabbitImages);
    m_Rabbit->SetSkillImages(rabbitSkillImages);  // 設置技能動畫圖片
    m_Rabbit->SetPosition({-112.5f, -140.5f});
    m_Rabbit->SetZIndex(50);
    m_Root.AddChild(m_Rabbit);

    // // 初始化敵人，設定圖片、位置與初始可見狀態
    m_Enemy = std::make_shared<Enemy>(100,1,std::vector<std::string>{GA_RESOURCE_DIR"/Image/Enemy/training_dummy_anim.png"});
    m_Enemy->SetZIndex(5);
    m_Enemy->SetPosition({197.5f, -3.5f});
    // m_Enemy->SetVisible(false); // 初始狀態為不可見
    m_Root.AddChild(m_Enemy);

    m_Background = std::make_shared<BackgroundImage>(); // 實際賦值
    m_Root.AddChild(m_Background);

    m_CurrentState = State::UPDATE;
}
