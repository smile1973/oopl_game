#ifndef BACKGROUND_IMAGE_HPP
#define BACKGROUND_IMAGE_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class BackgroundImage : public Util::GameObject {
public:
    BackgroundImage() : GameObject(
        std::make_unique<Util::Image>(GA_RESOURCE_DIR "/Image/Background/bg_black.png"), -10) {
    }

    void SetBackground(int mainPhase) const {
        auto temp = std::dynamic_pointer_cast<Util::Image>(m_Drawable);
        temp->SetImage(ImagePath(mainPhase));
    }

private:
    static std::string ImagePath(const int mainPhase) {
        switch (mainPhase) {
            case 1: return GA_RESOURCE_DIR "/Image/Background/stage_bg_0000.png";
            case 2: return GA_RESOURCE_DIR "/Image/Background/stage_bg_0001.png";
            case 3: return GA_RESOURCE_DIR "/Image/Background/stage_bg_0002.png";
            case 4: return GA_RESOURCE_DIR "/Image/Background/bg_3.png";
            case 5: return GA_RESOURCE_DIR "/Image/Background/bg_4.png";
            case 6: return GA_RESOURCE_DIR "/Image/Background/bg_5.png";
            default: return GA_RESOURCE_DIR "/Image/Background/bg_black.png";
        }
    }
};

#endif
