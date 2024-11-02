#include "core_module.h"

#include <iostream>
#include <cstdlib>

#include "SDL2/SDL.h"

core_model::core_model(){
    // 初始化 SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
        std::cout << "SDL 初始化失败！SDL_Error: " << SDL_GetError() << std::endl;
    }
    else{
        is_initialized = true;
    }
}

core_model::~core_model(){
    SDL_Quit();
    is_initialized = false;
}

void core_model::get_sdl_version(){
    // 获取 SDL 的编译时版本
    SDL_version compiled;
    SDL_VERSION(&compiled);
    std::cout << "SDL 的编译时版本为："
              << static_cast<int>(compiled.major) << "."
              << static_cast<int>(compiled.minor) << "."
              << static_cast<int>(compiled.patch) << std::endl;

    // 获取 SDL 的运行时版本
    SDL_version linked;
    SDL_GetVersion(&linked);
    std::cout << "SDL 的运行时版本为："
              << static_cast<int>(linked.major) << "."
              << static_cast<int>(linked.minor) << "."
              << static_cast<int>(linked.patch) << std::endl;
}

void core_model::sdl_log(){
    // 设置日志输出级别
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

    // 输出日志
    SDL_Log("SDL 日志输出测试！");

    // 输出错误日志
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL 错误日志输出测试！");

    // 输出警告日志
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "SDL 警告日志输出测试！");

    // 输出调试日志
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "SDL 调试日志输出测试！");

    // 输出信息日志
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "SDL 信息日志输出测试！");
}

void core_model::sdl_error(){
    // 获取错误信息
    const char* error = SDL_GetError();
    if (*error != '\0') {
        std::cout << "SDL Error: " << error << std::endl;
        // 清除错误信息
        SDL_ClearError();
    }
}

void core_model::sdl_platform(){
    // 获取平台信息
    SDL_Log("SDL 平台信息：%s", SDL_GetPlatform());
}

void core_model::sdl_memory(){
    // 使用 SDL_malloc 分配内存
    void* ptr = SDL_malloc(100);
    if (ptr == nullptr) {
        sdl_error();
        return;
    }
    std::cout << "使用 SDL_malloc 分配内存" << std::endl;

    // 使用 SDL_calloc 分配并初始化内存
    void* cptr = SDL_calloc(10, 10);
    if (cptr == nullptr) {
        sdl_error();
        SDL_free(ptr);
        return;
    }
    std::cout << "使用 SDL_calloc 分配并初始化内存" << std::endl;

    // 使用 SDL_realloc 重新分配内存
    ptr = SDL_realloc(ptr, 200);
    if (ptr == nullptr) {
        sdl_error();
        SDL_free(cptr);
        return;
    }
    std::cout << "使用 SDL_realloc 重新分配内存" << std::endl;

    // 使用 aligned_alloc 分配对齐的内存
    void* aptr = aligned_alloc(16, 256);
    if (aptr == nullptr) {
        sdl_error();
        SDL_free(ptr);
        SDL_free(cptr);
        return;
    }
    std::cout << "使用 aligned_alloc 分配对齐的内存" << std::endl;

    // 释放内存
    SDL_free(ptr);
    SDL_free(cptr);
    free(aptr);
    std::cout << "使用 SDL_free 和 free 释放内存" << std::endl;
}

void core_model::sdl_atomic(){
    SDL_atomic_t atomic_var;
    SDL_AtomicSet(&atomic_var, 0);
    std::cout << "原子变量初始值: " << SDL_AtomicGet(&atomic_var) << std::endl;

    SDL_AtomicAdd(&atomic_var, 10);
    std::cout << "原子变量加 10 后的值: " << SDL_AtomicGet(&atomic_var) << std::endl;

    if (SDL_AtomicCAS(&atomic_var, 10, 20)) {
        std::cout << "原子变量值从 10 成功交换为 20" << std::endl;
    } else {
        std::cout << "原子变量值交换失败" << std::endl;
    }

    std::cout << "原子变量最终值: " << SDL_AtomicGet(&atomic_var) << std::endl;
}