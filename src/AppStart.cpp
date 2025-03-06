#include "App.hpp"

#include "Util/Logger.hpp"
#include "Effect/EffectManager.hpp"

void App::Start() {
    LOG_TRACE("Start");
    Effect::EffectManager::GetInstance().Initialize(4, 4);

    // 將特效管理器添加到渲染樹
    m_Root.AddChild(std::shared_ptr<Util::GameObject>(&Effect::EffectManager::GetInstance(), [](Util::GameObject*){}));

    // 兔子閒置動畫
    std::vector<std::string> rabbitImages;
    rabbitImages.reserve(2);
    for (int i = 0; i < 2; ++i) {
        rabbitImages.emplace_back(GA_RESOURCE_DIR"/Image/Character/hb_rabbit_idle" + std::to_string(i+1) + ".png");
    }
    m_Rabbit = std::make_shared<Character>(rabbitImages);

    // 技能 Z 動畫
    std::vector<std::string> rabbitSkill1Images;
    rabbitSkill1Images.reserve(6);
    for (int i = 0; i < 6; ++i) {
        rabbitSkill1Images.emplace_back(GA_RESOURCE_DIR"/Image/Character/hb_rabbit_skill1_" + std::to_string(i+1) + ".png");
    }
    m_Rabbit->AddSkill(1, rabbitSkill1Images, 175, 0.45f, Util::Color(1.0f, 1.0f, 1.0f, 0.05f));  // Z鍵技能，ID=1
    m_Rabbit->AddSkill(3, rabbitSkill1Images, 175, 0.4f, Util::Color(1.0f, 1.0f, 1.0f, 0.05f));  // C鍵技能，ID=1

    // 技能 X 動畫
    std::vector<std::string> rabbitSkill2Images;
    rabbitSkill2Images.reserve(5);
    for (int i = 0; i < 5; ++i) {
        rabbitSkill2Images.emplace_back(GA_RESOURCE_DIR"/Image/Character/hb_rabbit_skill2_" + std::to_string(i+1) + ".png");
    }
    m_Rabbit->AddSkill(2, rabbitSkill2Images, 175, 0.4f, Util::Color(0.0f, 1.0f, 1.0f, 0.3f));  // X鍵技能，ID=2

    // 技能 V 動畫
    std::vector<std::string> rabbitSkill4Images;
    rabbitSkill4Images.reserve(4);
    for (int i = 0; i < 4; ++i) {
        rabbitSkill4Images.emplace_back(GA_RESOURCE_DIR"/Image/Character/hb_rabbit_skill3_" + std::to_string(i+1) + ".png");
    }
    m_Rabbit->AddSkill(4, rabbitSkill4Images, 175, 0.4f, Util::Color::FromName(Util::Colors::CYAN));  // V鍵技能，ID=4

    m_Rabbit->SetPosition({-112.5f, -140.5f});
    m_Rabbit->SetZIndex(50);
    m_Root.AddChild(m_Rabbit);

    m_Background = std::make_shared<BackgroundImage>();
    m_Root.AddChild(m_Background);

    m_CurrentState = State::UPDATE;
}
