#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export

#include "Util/Renderer.hpp"
#include "Character.hpp"
#include "Enemy.hpp"
#include "BackGround.hpp"
#include "Effect/EffectManager.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:


//private:
//    enum class Phase {
//        CHANGE_CHARACTER_IMAGE,
//        ABLE_TO_MOVE,
//        COLLIDE_DETECTION,
//        BEE_ANIMATION,
//        OPEN_THE_DOORS,
//        COUNTDOWN,
//    };


    State m_CurrentState = State::START;

    Util::Renderer m_Root;
    std::shared_ptr<BackgroundImage> m_Background; // 定義背景類
    std::shared_ptr<Character> m_Rabbit; // 定義兔子
    std::shared_ptr<Enemy> m_Enemy;   // 定義敵人

    bool m_EnterDown = false;
    bool m_ZKeyDown = false;
    bool m_XKeyDown = false;
    bool m_CKeyDown = false;
    bool m_VKeyDown = false;
    float m_TestEffectTimer = 0.0f;
};

#endif
