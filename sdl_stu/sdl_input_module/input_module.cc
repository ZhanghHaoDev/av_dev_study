#include "input_module.h"

#include <iostream>

#include "SDL.h"

input_module::input_module(){
    SDL_Init(SDL_INIT_EVENTS);
}

input_module::~input_module(){
    SDL_Quit();
}

void input_module::sdl_input(){
    SDL_Event event;
    bool quit = false;

    while (!quit) {
        // 1. 获取键盘状态
        const Uint8* state = SDL_GetKeyboardState(NULL);

        // 2. 判断按键状态
        if (state[SDL_SCANCODE_ESCAPE]) {
            std::cout << "Escape key is pressed." << std::endl;
            quit = true;
        }

        // 3. 处理键盘事件
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    std::cout << "Key pressed: " << SDL_GetKeyName(event.key.keysym.sym) << std::endl;
                    break;
                case SDL_KEYUP:
                    std::cout << "Key released: " << SDL_GetKeyName(event.key.keysym.sym) << std::endl;
                    break;
                case SDL_QUIT:
                    quit = true;
                    break;
                default:
                    break;
            }
        }

        // 延迟以减少 CPU 使用率
        SDL_Delay(10);
    }
}
