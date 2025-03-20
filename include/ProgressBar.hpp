#ifndef PROGRESS_BAR_HPP
#define PROGRESS_BAR_HPP

#include "Util/GameObject.hpp"
#include "ProgressIcon.hpp"

#include <vector>

class ProgressBar {
public:
    ProgressBar() {
        std::vector<std::string> iconNames = {"R", "battle", "battle", "battle", "treasure", "boss"};

        for (int i = 0; i < iconNames.size(); ++i) {
            constexpr float iconSpacing = 130.0f;
            m_Icons.push_back(std::make_shared<ProgressIcon>(iconNames[i]));
            m_Icons[i]->SetPosition(m_BasePosition + glm::vec2(iconSpacing * (i + 0.0), 0.0f));
        }
    }

    [[nodiscard]] std::vector<std::shared_ptr<Util::GameObject>> GetChildren() const {
        std::vector<std::shared_ptr<Util::GameObject>> children;
        for (const auto& icon : m_Icons) {
            children.push_back(icon);
        }
        return children;
    }

private:
    glm::vec2 m_BasePosition = {-450, 300};
    std::vector<std::shared_ptr<ProgressIcon>> m_Icons;
};

#endif // PROGRESS_BAR_HPP