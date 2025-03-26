#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export

#include "Util/Renderer.hpp"
#include "Character.hpp"
#include "Enemy.hpp"
#include "PhaseManger.hpp" // 階段資源管理
#include "Effect/EffectManager.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    [[nodiscard]] State GetCurrentState() const { return m_CurrentState; }

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    void GetReady() ;

    // 執行有效的任務，內部函式
    void ValidTask() const;
    void LeavePhase() const;
    void SetSubPhase() const;      // 設置關卡配置
    void SetupStorePhase() const;      // 設置商店關卡配置
    void SetupTreasurePhase() const;      // 設置寶箱關卡配置
    void SetupBattlePhase() const;      // 設置戰鬥關卡配置

    // // 大關類型
    // enum class MainPhase {
    //     INITIAL_SCENE,        // 初始場景
    //     KINGDOM_OUTSKIRTS,    // 第1大關
    //     SCHOLARS_NEST,        // 第2大關
    //     KINGS_ARSENAL,        // 第3大關
    //     RED_DARKHOUSE,        // 第4大關
    //     CHURCHMOUSE_STREETS   // 第5大關
    // };
    //
    // // 小關類型
    // enum class SubPhase {
    //     STORE,      // 商店
    //     BATTLE,     // 戰鬥
    //     TREASURE,   // 寶箱
    //     BOSS        // 頭目戰
    // };

    State m_CurrentState = State::START;
    // MainPhase m_MainPhase = MainPhase::INITIAL_SCENE;  // 當前大關
    // int m_SubPhaseIndex = 0;                           // 當前小關索引 (0-4)
    // SubPhase m_CurrentSubPhase = SubPhase::BATTLE;     // 當前小關類型

    Util::Renderer m_Root;
    std::shared_ptr<Character> m_Rabbit;               // 定義兔子
    std::shared_ptr<Enemy> m_Enemy;                    // 定義敵人
    std::shared_ptr<Enemy> m_Enemy_bird_valedictorian; // 定義敵人
    std::shared_ptr<Enemy> m_Enemy_dragon_silver;      // 定義敵人
    std::shared_ptr<Enemy> m_Enemy_treasure;           // 定義寶箱
    std::shared_ptr<Enemy> m_Enemy_shopkeeper;         // 定義商人
    std::shared_ptr<PhaseManager> m_PRM;               // 階段資源管理器

    bool m_EnterDown = false;
    bool m_ZKeyDown = false;
    bool m_XKeyDown = false;
    bool m_CKeyDown = false;
    bool m_VKeyDown = false;
    float m_TestEffectTimer = 0.0f;

    // 測試關卡切換
    bool m_NKeyDown = false;
    std::shared_ptr<Enemy> m_Onward;
    std::shared_ptr<Enemy> m_GetReady;
    std::shared_ptr<Enemy> m_PressZtoJoin;
    bool m_IsReady = false;
};

#endif
