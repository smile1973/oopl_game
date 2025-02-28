// Skill.hpp
#ifndef SKILL_HPP
#define SKILL_HPP

#include "Util/Animation.hpp"
#include "Effect/CircleEffect.hpp"
#include "Util/Color.hpp"

class Skill {
public:
    enum class State {
        IDLE,
        ACTIVE
    };

    // 構造函數，現在包含效果參數
    Skill(const std::vector<std::string>& imageSet, int duration = 175,
          float effectRadius = 0.4f,
          const Util::Color& effectColor = Util::Color::FromName(Util::Colors::WHITE));

    // 取得動畫物件
    std::shared_ptr<Util::Animation> GetAnimation() const { return m_Animation; }

    // 取得效果物件
    std::shared_ptr<Util::CircleEffect> GetEffect() const { return m_Effect; }

    // 播放技能動畫和效果
    void Play();

    // 檢查動畫是否已結束
    bool IsEnded() const;

    // 取得動畫路徑集
    const std::vector<std::string>& GetImagePathSet() const { return m_ImagePathSet; }

    // 更新技能狀態
    void Update(float deltaTime);

    // 繪製技能效果
    void DrawEffect(const Core::Matrices& data);

    // 取得技能當前狀態
    State GetState() const { return m_State; }

    // 設置效果的參數
    void SetEffectRadius(float radius) {
        if (m_Effect) m_Effect->SetRadius(radius);
    }

    void SetEffectColor(const Util::Color& color) {
        if (m_Effect) m_Effect->SetColor(color);
    }

    void SetEffectThickness(float thickness) {
        if (m_Effect) m_Effect->SetThickness(thickness);
    }

    void SetEffectFade(float fade) {
        if (m_Effect) m_Effect->SetFade(fade);
    }

private:
    std::vector<std::string> m_ImagePathSet;
    std::shared_ptr<Util::Animation> m_Animation;
    std::shared_ptr<Util::CircleEffect> m_Effect;
    State m_State = State::IDLE;
    int m_Duration = 175; // 技能動畫持續時間（毫秒）
    bool m_UseEffect = true; // 是否使用效果
};

#endif //SKILL_HPP