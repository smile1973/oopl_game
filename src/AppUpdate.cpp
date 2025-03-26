#include "App.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/logger.hpp"
#include "Util/Time.hpp"
#include "Effect/EffectManager.hpp"
#include "Effect/EffectFactory.hpp"

void App::Update() {
    if (!m_IsReady) {
        GetReady();
        return;
    }


    // 處理空格鍵 - 測試特效
    if (Util::Input::IsKeyDown(Util::Keycode::SPACE)) {
        auto cursorPos = Util::Input::GetCursorPosition();

        Effect::EffectType effectType;
        effectType = Effect::EffectType::ENEMY_ATTACK_1;
        LOG_DEBUG("Testing enemy attack 1 effect");

        auto effect = Effect::EffectManager::GetInstance().PlayEffect(
            effectType,
            cursorPos,
            10.0f, // z-index
            1.5f   // 持續時間
        );
        LOG_DEBUG("Created effect at cursor position: ({}, {})", cursorPos.x, cursorPos.y);
    }

    // 角色移動
    constexpr float moveSpeed = 6.0f; // 調整移動速度
    auto rabbitPos = m_Rabbit->GetPosition(); // 取得當前位置

    if (Util::Input::IsKeyPressed(Util::Keycode::UP)) {
        rabbitPos.y += moveSpeed; // 向上移動
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::DOWN)) {
        rabbitPos.y -= moveSpeed; // 向下移動
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        rabbitPos.x -= moveSpeed; // 向左移動
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
        rabbitPos.x += moveSpeed; // 向右移動
    }
    m_Rabbit->SetPosition(rabbitPos); // 更新位置

    // 退出檢查
    if (Util::Input::IsKeyPressed(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    // 初始化敵人容器
    std::vector<std::shared_ptr<Enemy>> m_Enemies;
    m_Enemies.push_back(m_Enemy);
    m_Enemies.push_back(m_Enemy_bird_valedictorian);
    m_Enemies.push_back(m_Enemy_dragon_silver);
    m_Enemies.push_back(m_Enemy_treasure);
    std::vector<std::shared_ptr<Character>> m_enemies_characters;
    for (const auto& enemy : m_Enemies) {
        m_enemies_characters.push_back(enemy); // 隱式轉換 std::shared_ptr<Enemy> 到 std::shared_ptr<Character>
    }

    // 技能Z
    if (m_ZKeyDown) {
        if (!Util::Input::IsKeyPressed(Util::Keycode::Z)) {
            LOG_DEBUG("Z Key UP - Skill 1");
            if (m_Rabbit->UseSkill(1)) {
                for (const auto& enemy : m_Enemies) {// 遍歷範圍內的敵人
                    if (m_Rabbit->IfCollides(enemy, 200)) {
                        enemy->TakeDamage(5);
                    }
                }
                m_Rabbit -> TowardNearestEnemy(m_enemies_characters);
            }
        }
    }
    m_ZKeyDown = Util::Input::IsKeyPressed(Util::Keycode::Z);

    // 技能X
    if (m_XKeyDown) {
        if (!Util::Input::IsKeyPressed(Util::Keycode::X)) {
            LOG_DEBUG("X Key UP - Skill 2");
            if (m_Rabbit->UseSkill(2)) {
                for (const auto& enemy : m_Enemies) {// 遍歷範圍內的敵人
                    if (m_Rabbit->IfCollides(enemy, 200)) {
                        enemy->TakeDamage(5);
                    }
                }
                m_Rabbit -> TowardNearestEnemy(m_enemies_characters);
            }
        }
    }
    m_XKeyDown = Util::Input::IsKeyPressed(Util::Keycode::X);

    // 技能C
    if (m_CKeyDown) {
        if (!Util::Input::IsKeyPressed(Util::Keycode::C)) {
            LOG_DEBUG("C Key UP - Skill 3");
            if (m_Rabbit->UseSkill(3)) {
                for (const auto& enemy : m_Enemies) {// 遍歷範圍內的敵人
                    if (m_Rabbit->IfCollides(enemy, 200)) {
                        enemy->TakeDamage(25);
                    }
                }
                m_Rabbit -> TowardNearestEnemy(m_enemies_characters);
            }
        }
    }
    m_CKeyDown = Util::Input::IsKeyPressed(Util::Keycode::C);

    // 技能V
    if (m_VKeyDown) {
        if (!Util::Input::IsKeyPressed(Util::Keycode::V)) {
            LOG_DEBUG("V Key UP - Skill 4");
            if (m_Rabbit->UseSkill(4)) {
                for (const auto& enemy : m_Enemies) {// 遍歷範圍內的敵人
                    if (m_Rabbit->IfCollides(enemy, 200)) {
                        enemy->TakeDamage(55);
                    }
                }
                m_Rabbit -> TowardNearestEnemy(m_enemies_characters);
            }
        }
    }
    m_VKeyDown = Util::Input::IsKeyPressed(Util::Keycode::V);

    // 更新特效管理器
    Effect::EffectManager::GetInstance().Update(Util::Time::GetDeltaTimeMs() / 1000.0f);

    // 更新兔子角色
    m_Rabbit->Update();

    // 更新敵人血條，是否允許前進
    for (const auto& enemy : m_Enemies) {// 遍歷範圍內的敵人
        enemy->DrawHealthBar();
    }
    if (Enemy::s_HealthBarYPositions.empty()) {
        m_Onward->SetVisible(true);
    } else {
        m_Onward->SetVisible(false);
        Enemy::s_HealthBarYPositions.clear();
    }

    // 關卡跳轉
    // if (m_Onward->GetVisibility() && m_Rabbit->IfCollides(m_Onward, 80)) {
    //     ValidTask();
    // }
    ValidTask();

    // 更新敵人角色
    m_Enemy->Update();
    m_PRM->Update();



    // 測試
    if (m_NKeyDown) {
        if (!Util::Input::IsKeyPressed(Util::Keycode::N)) {
            // m_Rabbit -> TowardNearestEnemy(m_enemies_characters);
            // m_Enemy->MoveToPosition(glm::vec2(1.0f, 0.0f), 3.0f);
            // m_Enemy->MovePosition(glm::vec2(-100.0f, 100.0f), 1.0f);
            for (const auto& enemy : m_Enemies) {// 遍歷範圍內的敵人
                if (m_Rabbit->IfCirclesCollide(enemy, 1000)) {
                    enemy->TakeDamage(1000);
                }}
            // SetSubPhase();
        }}
    m_NKeyDown = Util::Input::IsKeyPressed(Util::Keycode::N);




    if (Util::Input::IsKeyDown(Util::Keycode::I)) {
        for (int i = 0; i < 3; ++i) {
            auto eff = Effect::EffectManager::GetInstance().GetEffect(Effect::EffectType::ENEMY_ATTACK_1);
            eff->SetMovementModifier(Effect::Modifier::MovementModifier(true, 250.0f, 1200.0f, {0.0f, -1.0f}));
            eff->SetDuration(5.0f);
            eff->Play({-500 + (500 * i), 500}, 30.0f);
        }
    }

    // 測試矩形雷射特效 - 按下 1 鍵
    if (Util::Input::IsKeyDown(Util::Keycode::NUM_1)) {
        auto cursorPos = Util::Input::GetCursorPosition();
        auto effect = Effect::EffectManager::GetInstance().PlayEffect(
            Effect::EffectType::RECT_LASER,
            cursorPos,
            20.0f, // z-index
            2.0f   // 持續時間
        );
        LOG_DEBUG("Created RECT_LASER effect at position: ({}, {})", cursorPos.x, cursorPos.y);
    }

    // 測試矩形光束特效 - 按下 2 鍵
    if (Util::Input::IsKeyDown(Util::Keycode::NUM_2)) {
        auto cursorPos = Util::Input::GetCursorPosition();

        // 獲取標準自動旋轉特效
        auto effect1 = Effect::EffectManager::GetInstance().GetEffect(Effect::EffectType::RECT_BEAM);
        auto rectangleShape = std::dynamic_pointer_cast<Effect::Shape::RectangleShape>(effect1->GetBaseShape());
        rectangleShape->SetRotation(0.0f);
        effect1->SetDuration(10.0f);
        effect1->Play(cursorPos, 20.0f);
        LOG_DEBUG("Created standard auto-rotating RECT_BEAM effect at position: ({}, {})", cursorPos.x, cursorPos.y);

        // 創建一個新的自定義光束特效 - 從工廠獲取類似的基本特效
        auto effect2 = Effect::EffectManager::GetInstance().GetEffect(Effect::EffectType::RECT_BEAM);
        if (auto rectangleShape = std::dynamic_pointer_cast<Effect::Shape::RectangleShape>(effect2->GetBaseShape())) {
            // 設置90度旋轉 (π/2 弧度)
            rectangleShape->SetRotation(1.57f);
            effect2->SetDuration(10.0f);
            // 可選：調整旋轉速度
            // rectangleShape->SetAutoRotation(true, 1.0f);  // 減慢旋轉速度
        }

        // 放置在與第一個光束有點偏移的位置
        effect2->Play(cursorPos, 25.0f);
    }
    // 更新所有渲染對象
    m_Root.Update();
}