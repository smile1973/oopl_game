#ifndef DEFEAT_SCREEN_HPP
#define DEFEAT_SCREEN_HPP

#include "Util/GameObject.hpp"
#include "Character.hpp"
#include "Object.hpp"
#include "TextObject.hpp"

#include <string>
#include <sstream>
#include <iomanip>

class DefeatScreen {
public:
    DefeatScreen(const std::shared_ptr<Character>& character);

    [[nodiscard]] std::vector<std::shared_ptr<Util::GameObject>> GetChildren() const{return m_Children;};

    void SetVisible(bool visible);

    [[nodiscard]] int GetCurrentOption() const{ return m_CurrentOption; }

    [[nodiscard]] bool GetVisibility() const{ return m_IfVisible; }

    void Switch (bool isLeft);

    [[nodiscard]] bool IsLeaveGame() const{return m_CurrentOption==0;}

    void Reset() ;

    void Get();

    void Update();

private:
    bool m_IfVisible = false;
    std::shared_ptr<Character> m_Character; // 我們正在監控技能的角色參考

    std::vector<std::shared_ptr<Util::GameObject>> m_Children;

    std::shared_ptr<Object> m_BaseObject;
    std::shared_ptr<TextObject> m_GameTime;
    std::shared_ptr<TextObject> m_UserName;
    std::shared_ptr<TextObject> m_Level;
    std::shared_ptr<TextObject> m_DPS;

    std::vector<std::shared_ptr<Object>> m_Options;
    int m_CurrentOption = 0;
    float m_GameTimer = 0.0f;
    bool m_IsGameStart = false;

    static std::string StringGameTime(const float gameTime = 0) {
        const int totalMilliseconds = static_cast<int>(gameTime * 10); // 轉換成1/10豪秒的單位
        const int dms = totalMilliseconds % 10000;
        const int totalSeconds = totalMilliseconds / 10000;
        const int sec = totalSeconds % 60;
        const int totalMinutes = totalSeconds / 60;
        const int min = totalMinutes % 60;
        const int hr = totalMinutes / 60;

        std::stringstream ss;
        ss << std::setfill('0') << std::setw(1) << hr << ":"
           << std::setfill('0') << std::setw(2) << min << ":"
           << std::setfill('0') << std::setw(2) << sec << ":"
           << std::setfill('0') << std::setw(4) << dms;

        return ss.str();
    }
};

#endif // DEFEAT_SCREEN_HPP