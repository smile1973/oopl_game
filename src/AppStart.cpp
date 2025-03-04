#include "App.hpp"
#include <SDL.h>
#include "Util/Color.hpp"
#include "Util/Logger.hpp"

void drawRedRectangle(SDL_Renderer* renderer, int x, int y, int width, int height) {
    // 使用 Util::Color::FromRGB 創建紅色
    Util::Color red = Util::Color::FromRGB(255, 0, 0, 255); // 紅色，完全不透明
    // 設置 SDL 渲染器的繪圖顏色
    SDL_SetRenderDrawColor(renderer, red.r, red.g, red.b, red.a);
    // 創建 SDL_Rect 結構
    SDL_Rect rect = { x, y, width, height };
    // 使用渲染器的繪圖顏色填充矩形
    SDL_RenderFillRect(renderer, &rect);
}

void App::Start() {
    LOG_TRACE("Start");

    std::vector<std::string> rabbitImages;
    rabbitImages.reserve(2);
    for (int i = 0; i < 2; ++i) {
        rabbitImages.emplace_back(GA_RESOURCE_DIR"/Image/Character/hb_rabbit_idle" + std::to_string(i+1) + ".png");
    }

    std::vector<std::string> rabbitSkillImages;
    rabbitSkillImages.reserve(6);  // 假設有3張技能動畫圖片
    for (int i = 0; i < 6; ++i) {
        rabbitSkillImages.emplace_back(GA_RESOURCE_DIR"/Image/Character/hb_rabbit_skill1_" + std::to_string(i+1) + ".png");
    }

    m_Rabbit = std::make_shared<Character>(rabbitImages);
    m_Rabbit->SetSkillImages(rabbitSkillImages);  // 設置技能動畫圖片
    m_Rabbit->SetPosition({-112.5f, -140.5f});
    m_Rabbit->SetZIndex(50);
    m_Root.AddChild(m_Rabbit);

    // // 初始化敵人，設定圖片、位置與初始可見狀態
    m_Enemy = std::make_shared<Enemy>(100,1,std::vector<std::string>{GA_RESOURCE_DIR"/Image/Enemy/training_dummy_anim.png"});
    m_Enemy->SetZIndex(5);
    m_Enemy->SetPosition({197.5f, -3.5f});
    // m_Enemy->SetVisible(false); // 初始狀態為不可見
    m_Root.AddChild(m_Enemy);

    // 創建窗口
    SDL_Window* window = SDL_CreateWindow("Red Rectangle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    // 創建渲染器
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    // 繪製紅色長方形
    drawRedRectangle(renderer, 100, 100, 400, 10);
    SDL_RenderPresent(renderer);




    m_Background = std::make_shared<BackgroundImage>(); // 實際賦值
    m_Root.AddChild(m_Background);

    m_CurrentState = State::UPDATE;
}
