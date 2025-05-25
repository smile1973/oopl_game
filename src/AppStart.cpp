#include "App.hpp"

#include "Util/Logger.hpp"
#include "Effect/EffectManager.hpp"
#include "Attack/EnemyAttackController.hpp"
#include "Attack/AttackManager.hpp" // 添加攻擊管理器

void App::Start() {
    LOG_TRACE("Start");

    // 初始化特效管理器（預先創建10個每種類型的特效）
    Effect::EffectManager::GetInstance().Initialize(10);
    auto zEffect = Effect::EffectManager::GetInstance().GetEffect(Effect::EffectType::SKILL_Z);
    zEffect->Play({-9999, -9999}, -100);

    // 將特效管理器添加到渲染樹
    m_Root.AddChild(std::shared_ptr<Util::GameObject>(&Effect::EffectManager::GetInstance(), [](Util::GameObject*){}));

    std::vector<std::string> rabbitImages;
    rabbitImages.reserve(2);
    for (int i = 0; i < 2; ++i) {
        rabbitImages.emplace_back(GA_RESOURCE_DIR"/Image/Character/hb_rabbit_idle" + std::to_string(i+1) + ".png");
    }
    m_Rabbit = std::make_shared<Character>(rabbitImages);
    glm::vec2 scale = {0.5f, 0.5f};
    m_Rabbit -> m_Transform.scale = scale;

    std::vector<std::string> rabbitHurtImages;
    rabbitHurtImages.emplace_back(GA_RESOURCE_DIR"/Image/Character/hb_rabbit_gethit.png");
    m_Rabbit -> AddHurtAnimation(rabbitHurtImages, 500);

    // 技能 Z 動畫
    std::vector<std::string> rabbitSkill1Images;
    rabbitSkill1Images.reserve(6);
    for (int i = 0; i < 6; ++i) {
        rabbitSkill1Images.emplace_back(GA_RESOURCE_DIR"/Image/Character/hb_rabbit_skill1_" + std::to_string(i+1) + ".png");
    }
    m_Rabbit->AddSkill(1, rabbitSkill1Images, 175);  // Z鍵技能，ID=1
    m_Rabbit->AddSkill(3, rabbitSkill1Images, 175, 7.0f);   // C鍵技能，ID=3

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
    m_Rabbit->AddSkill(4, rabbitSkill4Images, 175, 12.0f);  // V鍵技能，ID=4

    m_Rabbit->SetPosition({-600.5f, -140.5f});
    m_Rabbit->SetZIndex(50);
    m_Rabbit->SetVisible(false);
    m_Root.AddChild(m_Rabbit);

    // 初始化敵人，設定圖片、位置與初始可見狀態
    m_Enemy = std::make_shared<Enemy>("dummy",100,std::vector<std::string>{GA_RESOURCE_DIR"/Image/Enemy/training_dummy_anim.png"});
    m_Enemy->SetShowHealthRing(true);
    m_Enemy->InitHealthRing();
    m_Root.AddChild(m_Enemy);





    // 初始化敵人攻擊控制器
    m_EnemyAttackController = std::make_shared<EnemyAttackController>(m_Enemy);

    m_Overlay = std::make_shared<Util::GameObject>(
    std::make_shared<Util::Image>(GA_RESOURCE_DIR "/Image/Background/overlay_black.png"), -9);
    m_Overlay->SetVisible(false);
    m_Root.AddChild(m_Overlay);

    m_Enemy_dummy = std::make_shared<Enemy>("dummy",100,std::vector<std::string>{GA_RESOURCE_DIR"/Image/Enemy/training_dummy_anim.png"});
    m_Enemy_dummy->SetShowHealthRing(true);
    m_Enemy_dummy->InitHealthRing();
    m_Root.AddChild(m_Enemy_dummy);

    // 初始化敵人(birdStudent)，設定圖片、位置與初始閒置狀態
    std::vector<std::string> birdStudent;
    birdStudent.reserve(7);
    for (int i = 7; i < 14; ++i) {
        birdStudent.emplace_back(GA_RESOURCE_DIR"/Image/Enemy/bird_student/bird_student_split_" + std::to_string(i+1) + ".png");
    }

    // 初始化敵人(birdWhispering)，設定圖片、位置與初始閒置狀態
    std::vector<std::string> birdWhispering;
    birdWhispering.reserve(7);
    for (int i = 7; i < 14; ++i) {
        birdWhispering.emplace_back(GA_RESOURCE_DIR"/Image/Enemy/bird_whispering/bird_whispering_split_" + std::to_string(i+1) + ".png");
    }

    // 初始化敵人(BirdValedictorian)，設定圖片、位置與初始閒置狀態
    std::vector<std::string> birdValedictorian;
    birdValedictorian.reserve(8);
    for (int i = 7; i >= 0; --i) {
        birdValedictorian.emplace_back(GA_RESOURCE_DIR"/Image/Enemy/bird_valedictorian/bird_valedictorian_split_" + std::to_string(i+1) + ".png");
    }

    // 初始化敵人(dragonGold)，設定圖片、位置與初始閒置狀態
    std::vector<std::string> dragonGold;
    dragonGold.reserve(7);
    for (int i = 7; i < 14; ++i) {
        dragonGold.emplace_back(GA_RESOURCE_DIR"/Image/Enemy/dragon_gold/dragon_gold_split_" + std::to_string(i+1) + ".png");
    }

    // 初始化敵人(dragonMythril)，設定圖片、位置與初始閒置狀態
    std::vector<std::string> dragonMythril;
    dragonMythril.reserve(7);
    for (int i = 7; i < 14; ++i) {
        dragonMythril.emplace_back(GA_RESOURCE_DIR"/Image/Enemy/dragon_mythril/dragon_mythril_split_" + std::to_string(i+1) + ".png");
    }

    // 初始化敵人(DragonSilver)，設定圖片、位置與初始閒置狀態
    std::vector<std::string> dragonSilver;
    dragonSilver.reserve(9);
    dragonSilver.emplace_back(GA_RESOURCE_DIR"/Image/Enemy/dragon_silver/dragon_silver_split_1.png");
    dragonSilver.emplace_back(GA_RESOURCE_DIR"/Image/Enemy/dragon_silver/dragon_silver_split_2.png");
    for (int i = 7; i < 14; ++i) {
        dragonSilver.emplace_back(GA_RESOURCE_DIR"/Image/Enemy/dragon_silver/dragon_silver_split_" + std::to_string(i+1) + ".png");
    }



    // 初始化敵人(mousePaladin)，設定圖片、位置與初始閒置狀態
    std::vector<std::string> mousePaladin;
    mousePaladin.reserve(7);
    for (int i = 7; i < 14; ++i) {
        mousePaladin.emplace_back(GA_RESOURCE_DIR"/Image/Enemy/mouse_paladin/mouse_paladin_split_" + std::to_string(i+1) + ".png");
    }

    // 初始化敵人(mouseRoseMage)，設定圖片、位置與初始閒置狀態
    std::vector<std::string> mouseRoseMage;
    // mouseRoseMage.reserve(7);
    for (int i = 7; i < 14; ++i) {
        mouseRoseMage.emplace_back(GA_RESOURCE_DIR"/Image/Enemy/mouse_rosemage/mouse_rosemage_split_" + std::to_string(i+1) + ".png");
    }

    // 初始化敵人(mouseCommander)，設定圖片、位置與初始閒置狀態
    std::vector<std::string> mouseCommander;
    mouseCommander.reserve(10);
    for (int i = 7; i < 17; ++i) {
        mouseCommander.emplace_back(GA_RESOURCE_DIR"/Image/Enemy/mouse_commander/mouse_commander_split_" + std::to_string(i+1) + ".png");
    }


    // 添加更多图片集
    m_Enemy->SetImageSetCollection(0, std::vector<std::string>{GA_RESOURCE_DIR"/Image/Enemy/training_dummy_anim.png"});

    m_Enemy->SetImageSetCollection(101, mousePaladin);
    m_Enemy->SetImageSetCollection(102, mouseRoseMage);
    m_Enemy->SetImageSetCollection(104, mouseCommander);

    m_Enemy->SetImageSetCollection(201, dragonGold);
    m_Enemy->SetImageSetCollection(202, dragonMythril);
    m_Enemy->SetImageSetCollection(204, dragonSilver);

    m_Enemy->SetImageSetCollection(301, birdStudent);
    m_Enemy->SetImageSetCollection(302, birdWhispering);
    m_Enemy->SetImageSetCollection(304, birdValedictorian);

    m_Enemy->m_Transform.scale.x = -0.5;



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

    m_PausedOption = std::make_shared<PausedScreen>();
    m_Root.AddChildren(m_PausedOption->GetChildren());

    m_DefeatScreen = std::make_shared<DefeatScreen>(m_Rabbit);
    m_Root.AddChildren(m_DefeatScreen->GetChildren());
    m_DefeatScreen->SetVisible(false);

    m_Onward = std::make_shared<Enemy>("Onward",1,std::vector<std::string>{GA_RESOURCE_DIR"/Image/Background/onward.png"});
    m_Onward->SetPosition({500.0f, 160.0f});
    m_Root.AddChild(m_Onward);

    m_GetReady = std::make_shared<Enemy>("GetReady",1,std::vector<std::string>{GA_RESOURCE_DIR"/Image/Background/get_ready.png"});
    m_GetReady->SetPosition({0.0f, 320.0f});
    m_GetReady->SetVisible(true);
    m_Root.AddChild(m_GetReady);

    m_PressZtoJoin = std::make_shared<Enemy>("GetReady",1,std::vector<std::string>{GA_RESOURCE_DIR"/Image/Background/press_Z_to_join.png"});
    m_PressZtoJoin->SetPosition({0.0f, 260.0f});
    m_PressZtoJoin -> m_Transform.scale =  {0.21f, 0.21f};
    m_PressZtoJoin->SetVisible(true);
    m_Root.AddChild(m_PressZtoJoin);

    m_SkillUI = std::make_shared<SkillUI>(m_Rabbit);
    m_Root.AddChildren(m_SkillUI->GetChildren());

    m_HealthBarUI = std::make_shared<HealthBarUI>(m_Rabbit);
    m_Root.AddChildren(m_HealthBarUI->GetChildren());

    m_LevelUI = std::make_shared<LevelUI>(m_Rabbit);
    m_Root.AddChildren(m_LevelUI->GetChildren());

    m_shopUI = std::make_shared<ShopUI>(m_Rabbit);
    m_Root.AddChildren(m_shopUI->GetChildren());

    m_CurrentState = State::UPDATE;

    LOG_INFO("Application started successfully");
}