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

    // 初始血量設置
    m_Health = 100;
    m_MaxHealth = 100;

    LOG_INFO("Character created with {} health", m_Health);
}

void Character::AddSkill(int skillId, const std::vector<std::string>& skillImageSet,
                        int duration, float Cooldown) {
    // 創建並儲存新技能
    auto newSkill = std::make_shared<Skill>(skillId, skillImageSet, duration, Cooldown);
    m_Skills[skillId] = newSkill;
    LOG_DEBUG("Added skill with ID: " + std::to_string(skillId));
}

bool Character::UseSkill(const int skillId, const std::vector<std::shared_ptr<Character>>& m_Enemies) {
    if (m_State == State::IDLE) {
        auto it = m_Skills.find(skillId);
        if (!it->second->IsOnCooldown()) {
            TowardNearestEnemy(m_Enemies, skillId==3);
            if (skillId == 3) {
                m_IsSkillCUes = true;
                m_Invincible = true;
                m_InvincibleTimer = 0.0f;
                m_InvincibleDuration = 0.75f;
            }else if (skillId == 4) {
                m_IsSkillVUes = true;
                // 進入無敵
                m_Invincible = true;
                m_InvincibleTimer = 0.0f;
                m_InvincibleDuration = 2.0f;
                SwitchToSkill(skillId);
            }else {
                SwitchToSkill(skillId);
            }
            return true;
        }
            
    }
    return false;
}
void Character::ResetSkill() {
    m_IsSkillXUes = false;
    m_IsSkillCUes = false;
    m_IsSkillVUes = false;
    LOG_INFO("X: " + std::to_string(m_IsSkillXUes) +
            " C: " + std::to_string(m_IsSkillCUes) +
            " V: " + std::to_string(m_IsSkillVUes));

    for (auto it = m_Skills.begin(); it != m_Skills.end(); ++it) {
        it->second->ResetCooldown();
    }
}


void Character::Update() {
    if (m_Invincible && !m_GodMode) {
        m_InvincibleTimer += Util::Time::GetDeltaTimeMs() / 1000.0f;
        if (m_InvincibleTimer >= m_InvincibleDuration) {
            m_Invincible = false;
            m_InvincibleTimer = 0.0f;
            LOG_DEBUG("Character invincibility ended");
            if (m_IsSkillVUes) {
                m_IsSkillVUes = false;
            }
            m_InvincibleDuration = 1.5f;
        }
    }

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
    else if (m_State == State::HURT) {
        // 更新受傷動畫計時器
        m_HurtAnimationTimer += Util::Time::GetDeltaTimeMs() / 1000.0f;

        // 如果受傷動畫結束，切回閒置狀態
        if (m_HurtAnimationTimer >= m_HurtAnimationDuration) {
            SwitchToIdle();
        }
    }

    UpdateLevel();

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
            // LOG_DEBUG("Character move to {}", m_Transform.translation);
        }
    }

    if (m_IsSkillCUes && !m_IsMoving) {
        SwitchToSkill(3);
        m_IsSkillCUes=false;
    }
}

void Character::SwitchToIdle() {
    if (m_State != State::IDLE) {
        // LOG_DEBUG("Switching to IDLE animation");
        m_State = State::IDLE;
        m_Drawable = m_IdleAnimation; // 直接切換到閒置動畫物件
        m_CurrentSkillId = -1;
        m_CurrentSkill = nullptr;
        m_HurtAnimationTimer = 0.0f;
    }
}

void Character::SwitchToSkill(int skillId) {
    auto it = m_Skills.find(skillId);
    if (it != m_Skills.end()) {
        // LOG_DEBUG("Switching to skill animation for skill ID: " + std::to_string(skillId));
        m_State = State::USING_SKILL;
        m_CurrentSkillId = skillId;
        m_CurrentSkill = it->second;

        // 播放技能動畫和效果 傳遞位置
        m_CurrentSkill->Play(m_Transform.translation, m_Transform.scale.x > 0.0f ? 1.0f : -1.0f);
        m_Drawable = m_CurrentSkill->GetAnimation();
    }
}

void Character::SwitchToHurt() {
    // LOG_DEBUG("Switching to HURT animation");
    m_State = State::HURT;
    m_HurtAnimationTimer = 0.0f;
    m_Drawable = m_HurtAnimation;
    m_HurtAnimation->Play();
}


void Character::TowardNearestEnemy(const std::vector<std::shared_ptr<Character>>& m_Enemies, const bool isMove) {
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
            m_Transform.scale.x = 0.5f;
        } else {
            m_Transform.scale.x = -0.5f;
        }

        if (isMove) {
            const float X = nearestEnemy->GetPosition().x - m_Transform.scale.x *180;
            const float Y = nearestEnemy->GetPosition().y;
            MoveToPosition(glm::vec2(X, Y), 0.2);
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
    m_MoveSpeed = (targetPosition - this->GetPosition()) / totalTime;
    m_TotalTime = totalTime * 1000.0f; //(ms)
}

void Character::TakeDamage(int damage) {
    // 如果無敵不受傷
    if (m_GodMode || m_Invincible || !IsAlive()) {
        LOG_DEBUG("Character is invincible or already defeated, damage ignored");
        return;
    }

    // 扣血
    m_Health = std::max(0, m_Health - damage);
    LOG_INFO("Character took {} damage, health: {}/{}", damage, m_Health, m_MaxHealth);

    // 進入無敵
    m_Invincible = true;
    m_InvincibleTimer = 0.0f;

    SwitchToHurt();

    // 死亡
    if (m_Health <= 0) {
        LOG_INFO("Character defeated!");
    }
}

void Character::SetMaxHealth(int maxHealth) {
    m_MaxHealth = maxHealth;
    m_Health = std::min(m_Health, m_MaxHealth); // 確保當前血量不超過最大值
    LOG_INFO("Character max health set to {}, current health: {}", m_MaxHealth, m_Health);
}

void Character::AddHurtAnimation(const std::vector<std::string>& hurtImageSet, int duration) {
    // 創建受傷動畫
    m_HurtAnimation = std::make_shared<Util::Animation>(hurtImageSet, true, duration, false, 0);
    m_HurtAnimationDuration = duration / 1000.0f;
}

void Character::UpdateLevel() {
    if (m_Experience >= 100) {
        m_Experience -= 100;
        m_Level++;
        LOG_INFO("Character Level Up {}  --Exp: {}", m_Level, m_Experience);
    }
}


void Character::Reset() {
    m_IsSkillXUes = false;
    m_IsSkillCUes = false;
    m_IsSkillVUes = false;

    m_CurrentSkillId = -1;

    // 血量相關屬性
    m_Health = m_MaxHealth;       // 最大血量
    m_Invincible = false; // 是否處於無敵狀態
    m_InvincibleTimer = 0.0f; // 無敵時間計時器
    m_InvincibleDuration = 1.5f; // 無敵時間(秒)

    // 受傷動畫相關
    m_HurtAnimationTimer = 0.0f;
    m_HurtAnimationDuration = 0.5f; // 受傷動畫持續時間(秒)

    // 移動相關屬性
    m_IsMoving = false;
    m_DistanceTraveled = 0.0f;
    m_TotalTime = 0.0f;
    m_TargetPosition = glm::vec2(0.0f, 0.0f);
    m_MoveSpeed = glm::vec2(0.0f, 0.0f);

    m_Transform.scale.x = 0.5;

    m_Money = 0;
    m_Experience = 0;
    m_Level = 1;
    SwitchToIdle();
    ResetSkill();
}

void Character::ToggleGodMode() {
    m_GodMode = !m_GodMode;
    if (m_GodMode) LOG_INFO("God Mode enabled - Character is now invincible");
    else LOG_INFO("God Mode disabled - Character returns to normal");
}
