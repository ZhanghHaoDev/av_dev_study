#include "image_module.h"

#include <iostream>

#include "SDL.h"
#include "SDL2/SDL_image.h"

image_module::image_module(){
    SDL_Init(SDL_INIT_EVENTS);
}

image_module::~image_module(){
    SDL_Quit();
}

void image_module::sdl_image_module() {
    // 初始化 SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return;
    }

    // 创建窗口
    SDL_Window* window = SDL_CreateWindow("SDL Image Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return;
    }

    // 创建渲染器
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return;
    }

    // 加载图片
    SDL_Surface* image_surface = IMG_Load("/Users/zhanghao/code/cpp/sdl_stu/assets/test.jpg");
    if (!image_surface) {
        std::cerr << "Unable to load image! IMG_Error: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return;
    }

    // 创建纹理
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image_surface);
    SDL_FreeSurface(image_surface);
    if (!texture) {
        std::cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return;
    }

    // 清除渲染器
    SDL_RenderClear(renderer);

    // 复制纹理到渲染器
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // 显示渲染结果
    SDL_RenderPresent(renderer);

    // 等待5秒钟
    SDL_Delay(5000);

    // 清理资源
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
}