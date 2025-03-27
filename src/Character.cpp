#include "Character.hpp"
#include "Util/Image.hpp"
#include "Util/Renderer.hpp"
#include "Util/Logger.hpp"
#include "Util/Time.hpp"
#include "Util/TransformUtils.hpp"

Character::Character(const std::vector<std::string>& ImagePathSet) {
    // 建立閒置動畫
    m_IdleAnimation = std::make_shared<Util::Animation>(ImagePathSet, true, 250, true, 0);
    // 初始時設置為閒置動畫
    m_Drawable = m_IdleAnimation;
    m_ImagePathSet = ImagePathSet;
    ResetPosition();
}

void Character::AddSkill(int skillId, const std::vector<std::string>& skillImageSet,
                        int duration, float Cooldown) {
    // 創建並儲存新技能
    auto newSkill = std::make_shared<Skill>(skillId, skillImageSet, duration, Cooldown);
    m_Skills[skillId] = newSkill;
    LOG_DEBUG("Added skill with ID: " + std::to_string(skillId));
}

bool Character::UseSkill(const int skillId) {
    if (m_State == State::IDLE) {
        // 檢查技能是否存在
        auto it = m_Skills.find(skillId);
        if (it != m_Skills.end()) {
            // LOG_DEBUG("Character using skill with ID: " + std::to_string(skillId));
            // SwitchToSkill(skillId);
            if (!it->second->IsOnCooldown()) {
                LOG_DEBUG("Character using skill with ID: " + std::to_string(skillId));
                SwitchToSkill(skillId);
                return true;
            }
            LOG_DEBUG("Skill with ID " + std::to_string(skillId) + " is on cooldown! " + std::to_string(it->second->GetRemainingCooldown()));
        } else {
            LOG_DEBUG("Skill with ID " + std::to_string(skillId) + " not found!");
        }
    }
    return false;
}

void Character::Update() {
    // 更新技能
    for (auto it = m_Skills.begin(); it != m_Skills.end(); ++it) {
        it->second->Update(Util::Time::GetDeltaTimeMs() / 1000.0f);
    }
    if (m_State == State::USING_SKILL && m_CurrentSkill) {
        // 檢查技能是否結束
        if (m_CurrentSkill->IsEnded()) {
            SwitchToIdle();
        }
    }

    // 移動位置
    if (m_IsMoving) {
        // 計算移動距離
        const float DeltaTimeMs = Util::Time::GetDeltaTimeMs();
        m_TotalTime -= DeltaTimeMs;
        // 更新位置
        m_Transform.translation += m_MoveSpeed * DeltaTimeMs / 1000.0f;
        if (m_TotalTime < 0.0f) {
            m_IsMoving = false; // 停止移動
            m_Transform.translation = m_TargetPosition;
            m_TotalTime = 0;
            LOG_DEBUG("Character move to {}", m_Transform.translation);
        }
    }
}

void Character::SwitchToIdle() {
    if (m_State != State::IDLE) {
        LOG_DEBUG("Switching to IDLE animation");
        m_State = State::IDLE;
        m_Drawable = m_IdleAnimation; // 直接切換到閒置動畫物件
        m_CurrentSkillId = -1;
        m_CurrentSkill = nullptr;
    }
}

void Character::SwitchToSkill(int skillId) {
    if (m_State == State::IDLE) {
        auto it = m_Skills.find(skillId);
        if (it != m_Skills.end()) {
            LOG_DEBUG("Switching to skill animation for skill ID: " + std::to_string(skillId));
            m_State = State::USING_SKILL;
            m_CurrentSkillId = skillId;
            m_CurrentSkill = it->second;

            // 播放技能動畫和效果 傳遞位置
            m_CurrentSkill->Play(m_Transform.translation);
            m_Drawable = m_CurrentSkill->GetAnimation();
        }
    }
}


void Character::TowardNearestEnemy(const std::vector<std::shared_ptr<Character>>& m_Enemies) {
    if (m_Enemies.empty()) return;

    float minDistance = std::numeric_limits<float>::max();
    std::shared_ptr<Character> nearestEnemy = nullptr;
    const glm::vec2 currentPosition = this->GetPosition();

    for (const auto& enemy : m_Enemies) {
        if (enemy->GetVisibility()) {
            float distance = glm::distance(currentPosition, enemy->GetPosition());
            if (distance < minDistance) {
                minDistance = distance;
                nearestEnemy = enemy;
            }
        }
    }

    if (nearestEnemy) {
        if (nearestEnemy->GetPosition().x > currentPosition.x) {
            LOG_DEBUG("Towards the Right");
            m_Transform.scale.x = 0.5f;
        } else {
            LOG_DEBUG("Towards the Left");
            m_Transform.scale.x = -0.5f;
        }
    }
}


bool Character::IsSkillOnCooldown(int skillId) const {
    auto it = m_Skills.find(skillId);
    if (it != m_Skills.end()) {
        return it->second->IsOnCooldown();
    }
    return false;
}

void Character::MovePosition(const glm::vec2& Position, const float totalTime) {
    MoveToPosition(GetPosition() + Position, totalTime);
}

void Character::MoveToPosition(const glm::vec2& targetPosition, const float totalTime) {
    if (totalTime <= 0.0f) {
        m_Transform.translation = targetPosition;
        return;
    }

    m_IsMoving = true;
    m_TargetPosition = targetPosition;
    // m_MaxDistance = glm::distance(this->GetPosition(), targetPosition);
    // m_Direction = (targetPosition - this->GetPosition()) / m_MaxDistance;
    // m_Speed = m_MaxDistance / totalTime;
    m_MoveSpeed = (targetPosition - this->GetPosition()) / totalTime;
    m_TotalTime = totalTime * 1000.0f; //(ms)
    LOG_DEBUG("Move Character to {}", m_Transform.translation);
}