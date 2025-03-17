#include "App.hpp"

#include "Util/Logger.hpp"
#include "Effect/EffectManager.hpp"

void App::Start() {
    LOG_TRACE("Start");

    // 初始化特效管理器（預先創建10個每種類型的特效）
    Effect::EffectManager::GetInstance().Initialize(10);

    // 問題:用Z之前 X V不能正常顯示(未解決)
    auto zEffect = Effect::EffectManager::GetInstance().GetEffect(Effect::EffectType::SKILL_Z);
    zEffect->Play({-9999, -9999}, -100);

    // 將特效管理器添加到渲染樹
    m_Root.AddChild(std::shared_ptr<Util::GameObject>(&Effect::EffectManager::GetInstance(), [](Util::GameObject*){}));

    // 兔子閒置動畫
    std::vector<std::string> rabbitImages;
    rabbitImages.reserve(2);
    for (int i = 0; i < 2; ++i) {
        rabbitImages.emplace_back(GA_RESOURCE_DIR"/Image/Character/hb_rabbit_idle" + std::to_string(i+1) + ".png");
    }
    m_Rabbit = std::make_shared<Character>(rabbitImages);
    glm::vec2 scale = {0.5f, 0.5f};
    m_Rabbit -> m_Transform.scale = scale;

    // 技能 Z 動畫
    std::vector<std::string> rabbitSkill1Images;
    rabbitSkill1Images.reserve(6);
    for (int i = 0; i < 6; ++i) {
        rabbitSkill1Images.emplace_back(GA_RESOURCE_DIR"/Image/Character/hb_rabbit_skill1_" + std::to_string(i+1) + ".png");
    }
    m_Rabbit->AddSkill(1, rabbitSkill1Images, 175);  // Z鍵技能，ID=1
    m_Rabbit->AddSkill(3, rabbitSkill1Images, 175);   // C鍵技能，ID=3

    // 技能 X 動畫
    std::vector<std::string> rabbitSkill2Images;
    rabbitSkill2Images.reserve(5);
    for (int i = 0; i < 5; ++i) {
        rabbitSkill2Images.emplace_back(GA_RESOURCE_DIR"/Image/Character/hb_rabbit_skill2_" + std::to_string(i+1) + ".png");
    }
    m_Rabbit->AddSkill(2, rabbitSkill2Images, 175);  // X鍵技能，ID=2

    // 技能 V 動畫
    std::vector<std::string> rabbitSkill4Images;
    rabbitSkill4Images.reserve(4);
    for (int i = 0; i < 4; ++i) {
        rabbitSkill4Images.emplace_back(GA_RESOURCE_DIR"/Image/Character/hb_rabbit_skill3_" + std::to_string(i+1) + ".png");
    }
    m_Rabbit->AddSkill(4, rabbitSkill4Images, 175);  // V鍵技能，ID=4

    m_Rabbit->SetPosition({-112.5f, -140.5f});
    m_Rabbit->SetZIndex(50);
    m_Root.AddChild(m_Rabbit);

    // 初始化敵人，設定圖片、位置與初始可見狀態
    m_Enemy = std::make_shared<Enemy>("dummy",100,std::vector<std::string>{GA_RESOURCE_DIR"/Image/Enemy/training_dummy_anim.png"});
    m_Root.AddChild(m_Enemy);

    // 初始化敵人(BirdValedictorian)，設定圖片、位置與初始閒置狀態
    std::vector<std::string> enemyImages;
    enemyImages.reserve(7);
    for (int i = 0; i < 7; ++i) {
        enemyImages.emplace_back(GA_RESOURCE_DIR"/Image/Enemy/bird_valedictorian/hb_bird_valedictorian_idle_" + std::to_string(i+1) + ".png");
    }
    m_Enemy_bird_valedictorian = std::make_shared<Enemy>("bird_valedictorian",120, enemyImages);
    m_Root.AddChild(m_Enemy_bird_valedictorian);

    // 初始化敵人(DragonSilver)，設定圖片、位置與初始閒置狀態
    std::vector<std::string> enemyImages2;
    enemyImages2.reserve(7);
    for (int i = 0; i < 7; ++i) {
        enemyImages2.emplace_back(GA_RESOURCE_DIR"/Image/Enemy/dragon_silver/dragon_silver_idle_" + std::to_string(i+1) + ".png");
    }
    m_Enemy_dragon_silver = std::make_shared<Enemy>("dragon_silver",120, enemyImages2);
    m_Root.AddChild(m_Enemy_dragon_silver);

    // 初始化敵人(Shopkeeper)，設定圖片、位置與初始閒置狀態
    std::vector<std::string> ShopkeeperImages2;
    ShopkeeperImages2.reserve(2);
    for (int i = 0; i < 2; ++i) {
        ShopkeeperImages2.emplace_back(GA_RESOURCE_DIR"/Image/Enemy/shopkeeper/cat_shopkeeper_split_" + std::to_string(i+1) + ".png");
    }
    m_Enemy_shopkeeper = std::make_shared<Enemy>("shopkeeper",120, ShopkeeperImages2);
    m_Enemy_shopkeeper->SetPosition({200.0f, -20.0f});
    m_Enemy_shopkeeper->SetInversion();
    m_Root.AddChild(m_Enemy_shopkeeper);

    // 初始化寶箱(Treasure)，設定圖片、位置與初始可見狀態
    m_Enemy_treasure = std::make_shared<Enemy>("treasure",30,std::vector<std::string>{GA_RESOURCE_DIR"/Image/Enemy/treasure.png"});
    m_Enemy_treasure->SetPosition({200.0f, -20.0f});
    m_Enemy_treasure->SetInversion();
    m_Root.AddChild(m_Enemy_treasure);


    m_PRM = std::make_shared<PhaseManager>();
    m_Root.AddChildren(m_PRM->GetChildren());

    m_Onward = std::make_shared<Enemy>("Onward",100,std::vector<std::string>{GA_RESOURCE_DIR"/Image/Background/onward.png"});
    m_Onward->SetPosition({420.0f, 180.0f});
    m_Onward->SetVisible(true);
    m_Root.AddChild(m_Onward);

    m_CurrentState = State::UPDATE;

    LOG_INFO("Application started successfully");
}