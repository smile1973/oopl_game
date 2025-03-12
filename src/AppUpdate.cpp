#include "App.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/logger.hpp"
#include "Util/Time.hpp"
#include "Effect/EffectManager.hpp"
#include "Effect/EffectFactory.hpp"

void App::Update() {
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
    const float moveSpeed = 6.0f; // 調整移動速度
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

    // 技能Z
    if (m_ZKeyDown) {
        if (!Util::Input::IsKeyPressed(Util::Keycode::Z)) {
            LOG_DEBUG("Z Key UP - Skill 1");
            m_Rabbit->UseSkill(1);

            if (m_Rabbit->IfCollides(m_Enemy, 150) && m_Enemy->IfAlive()) {
                m_Enemy->TakeDamage(5);

                if (! m_Enemy->IfAlive()) {
                    m_Enemy->SetVisible(false);
                    LOG_DEBUG("The Enemy dies");
                }
            }
        }
    }
    m_ZKeyDown = Util::Input::IsKeyPressed(Util::Keycode::Z);

    // 技能X
    if (m_XKeyDown) {
        if (!Util::Input::IsKeyPressed(Util::Keycode::X)) {
            LOG_DEBUG("X Key UP - Skill 2");
            m_Rabbit->UseSkill(2);
        }
    }
    m_XKeyDown = Util::Input::IsKeyPressed(Util::Keycode::X);

    // 技能C
    if (m_CKeyDown) {
        if (!Util::Input::IsKeyPressed(Util::Keycode::C)) {
            LOG_DEBUG("C Key UP - Skill 3");
            m_Rabbit->UseSkill(3);
        }
    }
    m_CKeyDown = Util::Input::IsKeyPressed(Util::Keycode::C);

    // 技能V
    if (m_VKeyDown) {
        if (!Util::Input::IsKeyPressed(Util::Keycode::V)) {
            LOG_DEBUG("V Key UP - Skill 4");
            m_Rabbit->UseSkill(4);
        }
    }
    m_VKeyDown = Util::Input::IsKeyPressed(Util::Keycode::V);

    // 更新特效管理器
    Effect::EffectManager::GetInstance().Update(Util::Time::GetDeltaTimeMs() / 1000.0f);

    // 更新兔子角色
    m_Rabbit->Update();

    // 更新敵人血條
    m_Enemy->DrawHealthBar(glm::vec2 (0.9f, 0.9));  // 繪製血條

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