#include "App.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/logger.hpp"
#include "Util/Time.hpp"
#include "Effect/EffectManager.hpp"
#include "Effect/EffectFactory.hpp"
#include "Attack/EnemyAttackController.hpp"
#include "Attack/AttackManager.hpp"
#include "Attack/CircleAttack.hpp"
#include "Attack/RectangleAttack.hpp"

void App::Update() {
    // 獲取時間增量
    const float deltaTime = Util::Time::GetDeltaTimeMs() / 1000.0f;

    if (!m_IsReady) {
        GetReady();
        return;
    }
    if (m_PRM->GetCurrentMainPhase()==1) {
        m_GetReady->SetVisible(false);
        m_Enemy_dummy->SetVisible(false);
    }
    if (m_PausedOption->GetVisibility() == true) {
        Pause();
        return;
    }
    if (m_DefeatScreen->GetVisibility() == true) {
        Defeat();
        return;
    }
    if (m_shopUI->GetVisibility() == true) {
        Shop();
        return;
    }


    // 角色移動
    constexpr float moveSpeed = 5.0f; // 調整移動速度
    auto rabbitPos = m_Rabbit->GetPosition(); // 取得當前位置
    // 定義邊界
    constexpr float minX = -600.0f;
    constexpr float maxX = 600.0f;
    constexpr float minY = -320.0f;
    constexpr float maxY = 320.0f;

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
    // 限制兔子在邊界內
    rabbitPos.x = std::max(minX, std::min(rabbitPos.x, maxX));
    rabbitPos.y = std::max(minY, std::min(rabbitPos.y, maxY));
    m_Rabbit->SetPosition(rabbitPos); // 更新位置

    // 退出
    if (Util::Input::IsKeyPressed(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    // 初始化敵人容器
    std::vector<std::shared_ptr<Enemy>> m_Enemies;
    m_Enemies.push_back(m_Enemy);
    m_Enemies.push_back(m_Enemy_treasure);
    m_Enemies.push_back(m_Enemy_dummy);
    std::vector<std::shared_ptr<Character>> m_enemies_characters;
    for (const auto& enemy : m_Enemies) {
        m_enemies_characters.push_back(enemy); // 隱式轉換 std::shared_ptr<Enemy> 到 std::shared_ptr<Character>
    }

    const int rabbitLevel = m_Rabbit->GetLevel();
    // 技能Z
    if (m_ZKeyDown) {
        if (!Util::Input::IsKeyPressed(Util::Keycode::Z)) {
            LOG_DEBUG("Z Key UP - Skill 1");
            if (m_Rabbit->UseSkill(1, m_enemies_characters)) {
                // m_Rabbit -> TowardNearestEnemy(m_enemies_characters);
                for (const auto& enemy : m_Enemies) {// 遍歷範圍內的敵人
                    if (m_Rabbit->IfCollideCircle(enemy, 200)) {
                        enemy->TakeDamage(10);
                        enemy->TakeDamage(m_Rabbit->IsSkillXUes() ? 1000 : 0);
                    }
                }
                m_Rabbit->UpdateSkillXUes(1);
            }
        }
    }
    m_ZKeyDown = Util::Input::IsKeyPressed(Util::Keycode::Z);

    // 技能X
    if (m_XKeyDown) {
        if (!Util::Input::IsKeyPressed(Util::Keycode::X)) {
            LOG_DEBUG("X Key UP - Skill 2");
            if (m_Rabbit->UseSkill(2, m_enemies_characters)) {
                for (const auto& enemy : m_Enemies) {// 遍歷範圍內的敵人
                    if (m_Rabbit->IfCollideSweptCircle(enemy)) {
                        enemy->TakeDamage(5*rabbitLevel);
                    }
                }
                m_Rabbit->UpdateSkillXUes(2);
            }
        }
    }
    m_XKeyDown = Util::Input::IsKeyPressed(Util::Keycode::X);

    // 技能C
    if (m_CKeyDown) {
        if (!Util::Input::IsKeyPressed(Util::Keycode::C)) {
            LOG_DEBUG("C Key UP - Skill 3");
            if (m_Rabbit->UseSkill(3, m_enemies_characters)) {
                // m_Rabbit -> TowardNearestEnemy(m_enemies_characters);
                for (const auto& enemy : m_Enemies) {// 遍歷範圍內的敵人
                    if (m_Rabbit->IfCollideEllipse(enemy) || true) {
                        enemy->TakeDamage(5*rabbitLevel);
                        enemy->TakeDamage(m_Rabbit->IsSkillXUes() ? 1000 : 0);
                    }
                }
                m_Rabbit->UpdateSkillXUes(3);
            }
        }
    }
    m_CKeyDown = Util::Input::IsKeyPressed(Util::Keycode::C);

    // 技能V
    if (m_VKeyDown) {
        if (!Util::Input::IsKeyPressed(Util::Keycode::V)) {
            LOG_DEBUG("V Key UP - Skill 4");
            if (m_Rabbit->UseSkill(4, m_enemies_characters)) {
                m_Rabbit -> TowardNearestEnemy(m_enemies_characters, false);
                m_Rabbit->UpdateSkillXUes(4);
            }
        }
    }
    m_VKeyDown = Util::Input::IsKeyPressed(Util::Keycode::V);

    // 更新攻擊控制器 (如果處於活動狀態)
    if (m_EnemyAttackController && m_Enemy->GetVisibility()) {
        m_EnemyAttackController->Update(deltaTime, m_Rabbit);
    }

    // 更新攻擊管理器
    AttackManager::GetInstance().Update(deltaTime, m_Rabbit);

    // 更新特效管理器
    Effect::EffectManager::GetInstance().Update(deltaTime);

    // 更新兔子角色
    m_Rabbit->Update();

    // 更新敵人血條，是否允許(前進)
    for (const auto& enemy : m_Enemies) {// 遍歷範圍內的敵人
        enemy->DrawHealthBar();
    }
    if (Enemy::s_HealthBarYPositions.empty()) {
        if (!m_Onward->GetVisibility()) {
            if (m_PRM->GetCurrentSubPhase()==1 || m_PRM->GetCurrentSubPhase()==2 || m_PRM->GetCurrentSubPhase()==4) {
                m_Rabbit->AddExperience(130);
                LOG_INFO("AddExperience(130)");
            }else if (m_PRM->GetCurrentSubPhase()==3) {
                m_Rabbit->AddMoney(30);
                LOG_INFO("AddMoney(30)");
            }
        }
        if (m_PRM->GetCurrentSubPhase()==4 && m_PRM->GetCurrentMainPhase()==3) {
            m_DefeatScreen -> Get(true);
        }
        m_Onward->SetVisible(true);
    } else {
        m_Onward->SetVisible(false);
        Enemy::s_HealthBarYPositions.clear();
    }

    ValidTask();

    // 更新敵人角色
    m_Enemy->Update();
    m_PRM->Update();

    m_Enemy_dummy->Update();
    m_SkillUI->Update();
    m_HealthBarUI->Update();
    m_LevelUI->Update();
    m_DefeatScreen->Update();


    // 測試
    if (m_NKeyDown) {
        if (!Util::Input::IsKeyPressed(Util::Keycode::N)) {
            Pause();
            LOG_DEBUG("--App::Pause--");
        }
    }
    m_NKeyDown = Util::Input::IsKeyPressed(Util::Keycode::N);

    if (m_GKeyDown) {
        if (!Util::Input::IsKeyPressed(Util::Keycode::G)) {
            if (m_Rabbit) {
                m_Rabbit->ToggleGodMode();
                // 更新 UI 顯示
                if (m_Rabbit->IsInGodMode()) {
                    LOG_DEBUG("God Mode enabled - Press G to disable");
                } else {
                    LOG_DEBUG("God Mode disabled");
                }
            }
        }
    }
    m_GKeyDown = Util::Input::IsKeyPressed(Util::Keycode::G);

    m_Root.Update();
}