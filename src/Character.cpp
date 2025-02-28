#include "Character.hpp"
#include "Util/Image.hpp"
#include "_Sprite.hpp"
#include "Util/Renderer.hpp"
#include "Util/Logger.hpp"
#include <glm/glm.hpp>

// Character 類別的建構函式，初始化角色的閒置動畫
Character::Character(const std::vector<std::string>& ImagePathSet){
    // 建立閒置動畫，設置為循環播放，每幀顯示 250 毫秒，並立即開始播放
    m_IdleAnimation = std::make_shared<Util::Animation>(ImagePathSet, true, 250, true, 0);
    m_Drawable = m_IdleAnimation;    // 設置初始動畫為閒置動畫
    m_ImagePathSet = ImagePathSet;    // 存儲圖片路徑集
    ResetPosition();    // 重置角色位置
}

// 設置角色技能動畫的圖片集，並建立對應的動畫物件
void Character::SetSkillImages(const std::vector<std::string>& SkillImageSet) {
    m_SkillImagePathSet = SkillImageSet;
    // 建立技能動畫，設置為非循環播放，每幀顯示 175 毫秒
    m_SkillAnimation = std::make_shared<Util::Animation>(SkillImageSet, true, 175, false, 0);
}

// 讓角色使用技能，觸發技能動畫
void Character::UseSkill() {
    if (m_State == State::IDLE) { // 只有當角色處於閒置狀態時才能施放技能
        LOG_DEBUG("Character using skill");
        SwitchToSkill(); // 切換到技能動畫
        // m_SkillAnimationTime = 0;
    }
}

// 更新角色狀態，例如檢查技能動畫是否結束
void Character::Update() {
    if (m_State == State::SKILL) { // 只有當角色處於技能狀態時才檢查
        auto skillAnim = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        // 若技能動畫播放結束，則切換回閒置動畫
        if (skillAnim && skillAnim->GetState() == Util::Animation::State::ENDED) {
            SwitchToIdle();
        }
    }
}

// 切換回閒置動畫
void Character::SwitchToIdle() {
    if (m_State != State::IDLE) { // 只有當當前狀態不是 IDLE 才切換
        LOG_DEBUG("Switching to IDLE animation");
        m_State = State::IDLE;
        m_Drawable = m_IdleAnimation; // 直接切換到閒置動畫物件
    }
}

// 切換到技能動畫
void Character::SwitchToSkill() {
    if (m_State != State::SKILL) { // 只有當當前狀態不是 SKILL 才切換
        LOG_DEBUG("Switching to SKILL animation");
        m_State = State::SKILL;
        m_SkillAnimation->Play(); // 播放技能動畫
        m_Drawable = m_SkillAnimation; // 設置動畫為技能動畫
    }
}

bool Character::IfCollides(const std::shared_ptr<Character>& other) const{
    if (!other) return false;

    // 取得當前角色和另一個角色的位置
    glm::vec2 pos1 = this->GetPosition();
    glm::vec2 pos2 = other->GetPosition();

    float size = 50.0f;

    // 簡單的 AABB (Axis-Aligned Bounding Box) 碰撞檢測
    bool isColliding = (abs(pos1.x - pos2.x) < size) && (abs(pos1.y - pos2.y) < size);

    return isColliding;
}