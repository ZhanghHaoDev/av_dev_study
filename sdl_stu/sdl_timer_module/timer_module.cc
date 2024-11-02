#include "timer_module.h"

#include "iostream"

#include "SDL.h"

time_module::time_module(){
    SDL_Init(SDL_INIT_TIMER);
}

time_module::~time_module(){
    SDL_Quit();
}

// 计时器回调函数
Uint32 timer_callback(Uint32 interval, void* param) {
    std::cout << "Timer callback called after " << interval << " milliseconds." << std::endl;
    return interval; // 返回值为0表示不重复，返回interval表示重复
}

void time_module::sdl_time_module() {
    // 创建计时器
    Uint32 interval = 1000; // 1秒
    SDL_TimerID timer_id = SDL_AddTimer(interval, timer_callback, nullptr);
    if (!timer_id) {
        std::cerr << "Failed to create timer: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return;
    }

    // 等待一段时间以观察计时器回调
    SDL_Delay(5000); // 等待5秒

    // 移除计时器
    SDL_RemoveTimer(timer_id);
}