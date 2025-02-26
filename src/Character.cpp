#include "Character.hpp"
#include "Util/Image.hpp"
#include "_Sprite.hpp"
#include "Util/Renderer.hpp"
#include "Util/Logger.hpp"

Character::Character(const std::vector<std::string>& ImagePathSet){
    // 建立閒置動畫
    m_IdleAnimation = std::make_shared<Util::Animation>(ImagePathSet, true, 250, true, 0);
    // 初始時設置為閒置動畫
    m_Drawable = m_IdleAnimation;
    m_ImagePathSet = ImagePathSet;
    ResetPosition();
}

void Character::SetSkillImages(const std::vector<std::string>& SkillImageSet) {
    m_SkillImagePathSet = SkillImageSet;
    // 提前創建技能動畫物件
    m_SkillAnimation = std::make_shared<Util::Animation>(SkillImageSet, true, 175, false, 0);
}

void Character::UseSkill() {
    if (m_State == State::IDLE) {
        LOG_DEBUG("Character using skill");
        SwitchToSkill();
        // m_SkillAnimationTime = 0;
    }
}

void Character::Update() {
    if (m_State == State::SKILL) {
        auto skillAnim = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        if (skillAnim && skillAnim-> GetState() == Util::Animation::State::ENDED) {
            SwitchToIdle();
        }
    }
}

void Character::SwitchToIdle() {
    if (m_State != State::IDLE) {
        LOG_DEBUG("Switching to IDLE animation");
        m_State = State::IDLE;
        m_Drawable = m_IdleAnimation; // 直接切換到閒置動畫物件
    }
}

void Character::SwitchToSkill() {
    if (m_State != State::SKILL) {
        LOG_DEBUG("Switching to SKILL animation");
        m_State = State::SKILL;
        m_SkillAnimation->Play();
        m_Drawable = m_SkillAnimation; // 直接切換到技能動畫物件
    }
}