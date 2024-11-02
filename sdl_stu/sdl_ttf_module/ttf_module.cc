#include "ttf_module.h"

#include <iostream>

#include "SDL.h"
#include "SDL2/SDL_ttf.h"

ttf_module::ttf_module(){
    SDL_Init(SDL_INIT_EVENTS);
}

ttf_module::~ttf_module(){
    SDL_Quit();
}

#include "ttf_module.h"
#include <iostream>

void ttf_module::sdl_ttf_module() {
    // 初始化 SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return;
    }

    // 创建窗口
    SDL_Window* window = SDL_CreateWindow("SDL TTF Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return;
    }

    // 创建渲染器
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return;
    }

    // 加载字体
    TTF_Font* font = TTF_OpenFont("/Users/zhanghao/code/cpp/sdl_stu/assets/QingNiaoHuaGuangJianMeiHei-2.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return;
    }

    // 创建字体颜色
    SDL_Color textColor = {255, 255, 255, 255}; // 白色

    // 渲染字体到表面
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Hello, SDL_ttf!", textColor);
    if (!textSurface) {
        std::cerr << "Unable to render text surface! TTF_Error: " << TTF_GetError() << std::endl;
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return;
    }

    // 创建纹理
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    if (!texture) {
        std::cerr << "Unable to create texture from rendered text! SDL_Error: " << SDL_GetError() << std::endl;
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return;
    }

    // 清除渲染器
    SDL_RenderClear(renderer);

    // 获取纹理大小
    int textWidth = 0, textHeight = 0;
    SDL_QueryTexture(texture, NULL, NULL, &textWidth, &textHeight);
    SDL_Rect renderQuad = { (800 - textWidth) / 2, (600 - textHeight) / 2, textWidth, textHeight };

    // 复制纹理到渲染器
    SDL_RenderCopy(renderer, texture, NULL, &renderQuad);

    // 显示渲染结果
    SDL_RenderPresent(renderer);

    // 等待5秒钟
    SDL_Delay(5000);

    // 清理资源
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
}