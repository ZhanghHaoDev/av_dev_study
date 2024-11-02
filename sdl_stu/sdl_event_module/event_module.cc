#include "event_module.h"
#include "SDL.h"
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <iostream>

event_module::event_module(){
    SDL_Init(SDL_INIT_EVENTS);
}

event_module::~event_module(){
    SDL_Quit();
}

void event_module::sdl_event(){
    // 创建窗口和渲染器
    SDL_Window *window = SDL_CreateWindow("SDL_event_loop",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,680,480,SDL_WINDOW_SHOWN);
    if(window == NULL){
        std::cerr << "sdl 窗口创建失败，错误码 " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Renderer *rendere = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    if(rendere == NULL){
        std::cerr << "sdl 渲染器创建失败，错误码 " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        return;
    }

    // 进入事件循环
    bool quit = false;
    SDL_Event e;

    while(!quit){
        // 从队列里面获取事件
        while(SDL_PollEvent(&e) != 0){
            // 根据事件进行处理
            if(e.type == SDL_QUIT){
                quit = true;
            }else if (e.type == SDL_KEYDOWN){
                // 输出按键
                std::cout << "key is " << SDL_GetKeyName(e.key.keysym.sym) << std::endl;
                if(e.key.keysym.sym == SDLK_ESCAPE){
                    quit = true;
                }
            }
        }
        // 更新窗口内容
        SDL_SetRenderDrawColor(rendere,0,255,0,0);// 白色背景
        SDL_RenderClear(rendere);
        SDL_RenderPresent(rendere);
    }

    // 退出事件循环
    SDL_DestroyRenderer(rendere);
    SDL_DestroyWindow(window);
}
