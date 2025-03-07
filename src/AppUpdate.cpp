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

        // 測試不同特效 - 根據當前測試計時器決定顯示哪種特效
        m_TestEffectTimer += Util::Time::GetDeltaTimeMs() / 1000.0f;
        if (m_TestEffectTimer > 7.0f) m_TestEffectTimer = 0.0f;

        Effect::EffectType effectType;
        if (m_TestEffectTimer < 1.0f) {
            effectType = Effect::EffectType::SKILL_Z;
            LOG_DEBUG("Testing Z skill effect");
        } else if (m_TestEffectTimer < 2.0f) {
            effectType = Effect::EffectType::SKILL_X;
            LOG_DEBUG("Testing X skill effect");
        } else if (m_TestEffectTimer < 3.0f) {
            effectType = Effect::EffectType::SKILL_C;
            LOG_DEBUG("Testing C skill effect");
        } else if (m_TestEffectTimer < 4.0f) {
            effectType = Effect::EffectType::SKILL_V;
            LOG_DEBUG("Testing V skill effect");
        } else if (m_TestEffectTimer < 5.0f) {
            effectType = Effect::EffectType::ENEMY_ATTACK_1;
            LOG_DEBUG("Testing enemy attack 1 effect");
        } else if (m_TestEffectTimer < 6.0f) {
            effectType = Effect::EffectType::ENEMY_ATTACK_2;
            LOG_DEBUG("Testing enemy attack 2 effect");
        } else {
            effectType = Effect::EffectType::ENEMY_ATTACK_3;
            LOG_DEBUG("Testing enemy attack 3 effect");
        }

        // 播放特效
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

    // 更新所有渲染對象
    m_Root.Update();

    // 顯示活躍特效數量(調試用)
    if (Util::Input::IsKeyDown(Util::Keycode::I)) {
        LOG_DEBUG("Active Effects: {}", Effect::EffectManager::GetInstance().GetActiveEffectsCount());
    }
}