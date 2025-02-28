#include "App.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/logger.hpp"

void App::Update() {
    const float moveSpeed = 8.0f; // 調整移動速度
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

    if (Util::Input::IsKeyPressed(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    // 技能Z
    if (m_ZKeyDown) {
        if (!Util::Input::IsKeyPressed(Util::Keycode::Z)) {
            LOG_DEBUG("Z Key UP - Skill 1");
            m_Rabbit->UseSkill(1);
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

    m_Rabbit->UpdateAndDraw();
    m_Root.Update();
}

