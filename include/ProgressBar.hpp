#ifndef PROGRESS_BAR_HPP
#define PROGRESS_BAR_HPP

#include "Util/GameObject.hpp"
#include "ProgressIcon.hpp"

#include <vector>

class ProgressBar {
public:
    ProgressBar() {
        std::vector<std::string> iconNames = {"store", "battle", "battle", "battle", "treasure", "boss", "R", "bar"};

        for (int i = 0; i < iconNames.size(); ++i) {
            constexpr float iconSpacing = 130.0f;
            m_Icons.push_back(std::make_shared<ProgressIcon>(iconNames[i]));
            m_Icons[i]->SetPosition(m_BasePosition + glm::vec2(iconSpacing * (i + 0.0), 0.0f));
        }
        m_Icons[6]->SetZIndex(32);
    }

    [[nodiscard]] std::vector<std::shared_ptr<Util::GameObject>> GetChildren() const {
        std::vector<std::shared_ptr<Util::GameObject>> children;
        for (const auto& icon : m_Icons) {
            children.push_back(icon);
        }
        return children;
    }

    void SetProgressBar(const int m_SubPhase)  {
        // for (const auto& icon : m_Icons) {
        //     icon->Update();
        // }
        // m_Icons[m_SubPhase-1]->SetVisible(false);
        m_Icons[6]->MoveToPosition(m_Icons[m_SubPhase]->GetPosition(), 1.2f);
        LOG_INFO("ProgressBar::SetProgressBar---{}",m_SubPhase);
    }

    bool IfSetComplete(const int m_SubPhase) {
        if (m_Icons[6]->GetPosition() != m_Icons[m_SubPhase]->GetPosition())return false;
        return true;
    }

    void Update() const {
        for (const auto& icon : m_Icons) {
            icon->Update();
            if (icon->GetPosition().x < m_Icons[6]->GetPosition().x) {
                icon->SetVisible(false);
            }else {
                icon->SetVisible(true);
            }
            // icon->SetVisible(icon->GetVisibility() && m_IfVisible);
        }
    }

private:
    bool m_IfVisible = false;
    glm::vec2 m_BasePosition = {-450, 300};
    std::vector<std::shared_ptr<ProgressIcon>> m_Icons;
};

#endif // PROGRESS_BAR_HPP