#include "thread_module.h"

#include <iostream>

#include "SDL.h"

thread_module::thread_module(){
    SDL_Init(SDL_INIT_EVENTS);
}

thread_module::~thread_module(){
    SDL_Quit();
}

// 线程函数
int thread_function(void* data) {
    const char* thread_name = static_cast<const char*>(data);
    for (int i = 0; i < 5; ++i) {
        std::cout << thread_name << " running: " << i << std::endl;
        SDL_Delay(1000); // 延迟1秒
    }
    return 0;
}

void thread_module::sdl_thread_module() {
    // 创建线程
    SDL_Thread* thread1 = SDL_CreateThread(thread_function, "Thread1", (void*)"Thread 1");
    SDL_Thread* thread2 = SDL_CreateThread(thread_function, "Thread2", (void*)"Thread 2");

    if (!thread1 || !thread2) {
        std::cerr << "Failed to create thread: " << SDL_GetError() << std::endl;
        return;
    }

    // 等待线程完成
    int thread1_return_value, thread2_return_value;
    SDL_WaitThread(thread1, &thread1_return_value);
    SDL_WaitThread(thread2, &thread2_return_value);

    std::cout << "Thread 1 returned value: " << thread1_return_value << std::endl;
    std::cout << "Thread 2 returned value: " << thread2_return_value << std::endl;
}