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

    if (m_ZKeyDown) {
        if (!Util::Input::IsKeyPressed(Util::Keycode::Z)) {
            LOG_DEBUG("ZKeyUP");
            m_Rabbit->UseSkill();

            if (m_Rabbit->IfCollides(m_Enemy)) {
                m_Enemy->TakeDamage(20);
                LOG_DEBUG("TakeDamage_20");
            }
        }
    }
    m_ZKeyDown = Util::Input::IsKeyPressed(Util::Keycode::Z);



    // if (m_EnterDown) {
    //     if (!Util::Input::IsKeyPressed(Util::Keycode::RETURN)) {
    //         LOG_DEBUG("EKeyUP");
    //         m_Rabbit->UseSkill();
    //     }
    // }
    // m_EnterDown = Util::Input::IsKeyPressed(Util::Keycode::RETURN);

    if (! m_Enemy->IsAlive()) {
        m_Enemy->SetVisible(false);
    }
    m_Rabbit->Update();
    m_Root.Update();
}
