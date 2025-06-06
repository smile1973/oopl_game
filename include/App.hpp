#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp"

#include "Util/Renderer.hpp"
#include "Character.hpp"
#include "Enemy.hpp"
#include "PhaseManger.hpp"
#include "PauseScreen.hpp"
#include "DefeatScreen.hpp"
#include "SkillUI.hpp"
#include "HealthBarUI.hpp"
#include "LevelUI.hpp"
#include "ShopUI.hpp"
#include "Effect/EffectManager.hpp"
#include "Attack/EnemyAttackController.hpp"
#include "Attack/AttackManager.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };
    static App& GetInstance() {
        static App instance;
        return instance;
    }

    // 禁止複製和賦值
    App(const App&) = delete;
    App& operator=(const App&) = delete;

    [[nodiscard]] State GetCurrentState() const { return m_CurrentState; }
    [[nodiscard]] std::shared_ptr<Util::GameObject> GetOverlay() const { return m_Overlay; }

    void Start();

    void Update();

    void End();

    void AddToRoot(const std::shared_ptr<Util::GameObject> &object) {
        m_Root.AddChild(object);
    }
    void RemoveFromRoot(const std::shared_ptr<Util::GameObject> &object) {
        m_Root.RemoveChild(object);
    }

private:
    void GetReady();
    void Pause();
    void Defeat();
    void Shop();

    void ValidTask();
    void LeavePhase() const;
    void SetSubPhase() const;      // 設置關卡配置
    void SetupStorePhase() const;      // 設置商店關卡配置
    void SetupTreasurePhase() const;      // 設置寶箱關卡配置
    void SetupBattlePhase() const;      // 設置戰鬥關卡配置
    void RestartGame();

    App() {}

    static App* s_Instance;

    State m_CurrentState = State::START;

    Util::Renderer m_Root;
    std::shared_ptr<PhaseManager> m_PRM; // 階段資源管理器
    std::shared_ptr<EnemyAttackController> m_EnemyAttackController; // 敵人攻擊控制器
    std::shared_ptr<Character> m_Rabbit;               // 定義兔子
    std::shared_ptr<Enemy> m_Enemy_dummy;              // 定義敵人_假人
    std::shared_ptr<Enemy> m_Enemy;                    // 定義敵人
    std::shared_ptr<Enemy> m_Enemy_treasure;           // 定義寶箱
    std::shared_ptr<Enemy> m_Enemy_shopkeeper;         // 定義商人
    std::shared_ptr<PausedScreen> m_PausedOption;      // 暫停畫面
    std::shared_ptr<DefeatScreen> m_DefeatScreen;      // 結算畫面
    std::shared_ptr<SkillUI> m_SkillUI;                // 角色技能UI
    std::shared_ptr<HealthBarUI> m_HealthBarUI;        // 角色血條UI
    std::shared_ptr<LevelUI> m_LevelUI;                // 角色等級UI
    std::shared_ptr<ShopUI> m_shopUI;                  // 商店UI
    std::shared_ptr<Util::GameObject> m_Overlay;

    bool m_EnterDown = false;
    bool m_ZKeyDown = false;
    bool m_XKeyDown = false;
    bool m_CKeyDown = false;
    bool m_VKeyDown = false;
    float m_TestEffectTimer = 0.0f;

    bool m_NKeyDown = false;
    bool m_RKeyDown = false;
    bool m_UpKeyDown = false;
    bool m_DownKeyDown = false;
    bool m_LeftKeyDown = false;
    bool m_RightKeyDown = false;
    std::shared_ptr<Enemy> m_Onward;
    std::shared_ptr<Enemy> m_GetReady;
    std::shared_ptr<Enemy> m_PressZtoJoin;
    bool m_IsReady = false;
    int m_CurrentPausedOption = 0;
    bool m_GKeyDown = false;
};

#endif