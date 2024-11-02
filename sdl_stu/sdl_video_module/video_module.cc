#include "video_module.h"

#include <iostream>
#include <string>
#include <unistd.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

video_module::video_module(){
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cout << "SDL 初始化失败！SDL_Error: " << SDL_GetError() << std::endl;
    }
    else{
        is_initialized = true;
        std::cout << "SDL 初始化成功！" << std::endl;
    }
}

video_module::~video_module(){
    SDL_Quit();
    is_initialized = false;
}

void video_module::sdl_window(){
    // 1. 创建窗口
    SDL_Window* my_window = SDL_CreateWindow("Initial Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!my_window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return;
    }

    // 暂停几秒钟以便观察窗口
    SDL_Delay(2000);

    // 2. 窗口大小调整
    SDL_SetWindowSize(my_window, 1024, 768);

    // 暂停几秒钟以便观察窗口
    SDL_Delay(2000);

    // 3. 窗口标题设置
    SDL_SetWindowTitle(my_window, "Updated Title");

    // 暂停几秒钟以便观察窗口
    SDL_Delay(2000);

    // 4. 窗口全屏模式
    if (SDL_SetWindowFullscreen(my_window, SDL_WINDOW_FULLSCREEN) != 0) {
        std::cerr << "Failed to set fullscreen mode: " << SDL_GetError() << std::endl;
    }

    // 暂停几秒钟以便观察窗口
    SDL_Delay(2000);

    // 5. 获取窗口信息，窗口位置
    int x, y, w, h;
    SDL_GetWindowPosition(my_window, &x, &y);
    SDL_GetWindowSize(my_window, &w, &h);
    std::cout << "Window position: (" << x << ", " << y << ")" << std::endl;
    std::cout << "Window size: (" << w << ", " << h << ")" << std::endl;

    // 暂停几秒钟以便观察窗口
    SDL_Delay(2000);

    // 6. 设置窗口大小
    SDL_SetWindowSize(my_window, 1280, 720);

    // 暂停几秒钟以便观察窗口
    SDL_Delay(2000);

    // 7. 销毁窗口
    SDL_DestroyWindow(my_window);
}

void video_module::render_and_draw(){
    // 1. 创建窗口
    SDL_Window* window = SDL_CreateWindow("Render and Draw", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return;
    }

    // 1. 创建渲染器
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        return;
    }

    // 2. 设置渲染颜色
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // 红色

    // 3. 清除渲染目标
    SDL_RenderClear(renderer);

    // 6. 绘制矩形
    SDL_Rect rect = { 100, 100, 200, 150 };
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // 绿色
    SDL_RenderDrawRect(renderer, &rect);

    // 7. 填充矩形
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // 蓝色
    SDL_RenderFillRect(renderer, &rect);

    // 8. 绘制线条
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // 黄色
    SDL_RenderDrawLine(renderer, 0, 0, 800, 600);

    // 9. 绘制点
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 白色
    SDL_RenderDrawPoint(renderer, 400, 300);

    // 4. 显示渲染结果
    SDL_RenderPresent(renderer);

    // 暂停几秒钟以便观察渲染结果
    SDL_Delay(5000);

    // 5. 销毁渲染器
    SDL_DestroyRenderer(renderer);

    // 7. 销毁窗口
    SDL_DestroyWindow(window);
}

void video_module::sdl_image(){
    // 1. 创建窗口
    SDL_Window* window = SDL_CreateWindow("SDL Image", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return;
    }

    // 1. 创建渲染器
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        return;
    }

    // 1. 加载图像
    SDL_Surface* surface = IMG_Load("/Users/zhanghao/code/cpp/sdl_stu/assets/test.jpg");
    if (!surface) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return;
    }

    // 2. 创建纹理
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return;
    }

    // 3. 释放表面
    SDL_FreeSurface(surface);

    // 5. 图像格式转换（示例：将图像转换为 32 位格式）
    SDL_Surface* converted_surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    if (!converted_surface) {
        std::cerr << "Failed to convert surface format: " << SDL_GetError() << std::endl;
    } else {
        SDL_FreeSurface(converted_surface);
    }

    // 6. 图像缩放（示例：将图像缩放到一半大小）
    SDL_Rect src_rect = { 0, 0, 800, 600 };
    SDL_Rect dst_rect = { 0, 0, 400, 300 };
    SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect);

    // 7. 图像旋转（SDL2 没有直接支持图像旋转的函数，通常需要使用 SDL2_gfx 库）
    // 这里假设你已经安装了 SDL2_gfx 库
    // SDL_RenderCopyEx(renderer, texture, &src_rect, &dst_rect, 45.0, NULL, SDL_FLIP_NONE);

    // 4. 显示渲染结果
    SDL_RenderPresent(renderer);

    // 暂停几秒钟以便观察渲染结果
    SDL_Delay(5000);

    // 4. 销毁纹理
    SDL_DestroyTexture(texture);

    // 5. 销毁渲染器
    SDL_DestroyRenderer(renderer);

    // 6. 销毁窗口
    SDL_DestroyWindow(window);
}
