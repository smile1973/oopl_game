#ifndef PROGRESS_ICON_HPP
#define PROGRESS_ICON_HPP

#include "Enemy.hpp"
#include "Util/Image.hpp"

class ProgressIcon : public Object {
public:
    explicit ProgressIcon(const std::string& imageName) : Object({IconImagePath(imageName)}) { // 修正建構子
        m_Transform.scale = {0.5f, 0.5f};
        SetZIndex(30);
        GameObject::SetVisible(false);
    }

    void SetProgressIcon(const std::string &imageName){
        m_Drawable = std::make_shared<Util::Image>(IconImagePath(imageName));
    }

private:
    static std::string IconImagePath(const std::string& imageName) {
        return GA_RESOURCE_DIR "/Image/ProgressBar/" + imageName + ".png";
    }
};

#endif // PROGRESS_ICON_HPP