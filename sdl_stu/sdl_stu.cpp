#include "sdl_stu.h"

#include <iostream>
#include <fstream>

#include "SDL.h"
#include "SDL_mixer.h"
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include "SDL_image.h"
#include "SDL_ttf.h"

sdl_stu::sdl_stu(){SDL_Init(SDL_INIT_EVERYTHING);}

sdl_stu::~sdl_stu() {SDL_Quit();};

void sdl_stu::sdl_audio_play(std::string file_path){
    // 初始化SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        SDL_Quit();
        return;
    }

    // 设置音频规格
    SDL_AudioSpec desiredSpec;
    SDL_AudioSpec obtainedSpec;
    desiredSpec.freq = 44100; // 采样率
    desiredSpec.format = AUDIO_F32; // 音频格式
    desiredSpec.channels = 2; // 通道数
    desiredSpec.samples = 4096; // 音频缓冲区大小
    desiredSpec.callback = nullptr; // 不使用回调函数

    // 打开音频设备
    if (SDL_OpenAudio(&desiredSpec, &obtainedSpec) < 0) {
        std::cerr << "SDL could not open audio! SDL_Error: " << SDL_GetError() << std::endl;
        Mix_CloseAudio();
        SDL_Quit();
        return;
    }

    // 加载音效
    Mix_Chunk* sound = Mix_LoadWAV(file_path.c_str());
    if (sound == NULL) {
        std::cerr << "Failed to load sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
        SDL_CloseAudio();
        Mix_CloseAudio();
        SDL_Quit();
        return;
    }

    // 加载音乐
    Mix_Music* music = Mix_LoadMUS(file_path.c_str());
    if (music == NULL) {
        std::cerr << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        Mix_FreeChunk(sound);
        SDL_CloseAudio();
        Mix_CloseAudio();
        SDL_Quit();
        return;
    }

    // 播放音效
    if (Mix_PlayChannel(-1, sound, 0) == -1) {
        std::cerr << "Failed to play sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }

    // 播放音乐
    if (Mix_PlayMusic(music, -1) == -1) {
        std::cerr << "Failed to play music! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }

    // 开始播放音频
    SDL_PauseAudio(0);

    // 主循环
    bool quit = false;
    while (!quit) {
        // 处理事件
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // 手动填充音频缓冲区
        static double phase = 0.0;
        double frequency = 440.0; // 频率（Hz）
        double sampleRate = 44100.0; // 采样率（Hz）
        double amplitude = 0.5; // 振幅

        int len = obtainedSpec.samples * obtainedSpec.channels * sizeof(float);
        Uint8* stream = new Uint8[len];
        SDL_memset(stream, 0, len);

        float* buffer = (float*)stream;
        int samples = len / sizeof(float);
        for (int i = 0; i < samples; ++i) {
            buffer[i] = amplitude * sin(phase * 2.0 * M_PI);
            phase += frequency / sampleRate;
            if (phase >= 1.0) {
                phase -= 1.0;
            }
        }

        SDL_QueueAudio(1, stream, len);
        delete[] stream;
    }

    // 释放音效
    Mix_FreeChunk(sound);
    sound = NULL;

    // 释放音乐
    Mix_FreeMusic(music);
    music = NULL;

    // 关闭音频设备
    SDL_CloseAudio();

    // 关闭SDL_mixer
    Mix_CloseAudio();
}

void sdl_stu::get_sdl_version(){
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

void sdl_stu::sdl_log(){
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

void sdl_stu::sdl_error(){
    // 获取错误信息
    const char* error = SDL_GetError();
    if (*error != '\0') {
        std::cout << "SDL Error: " << error << std::endl;
        // 清除错误信息
        SDL_ClearError();
    }
}

void sdl_stu::sdl_platform(){
    // 获取平台信息
    SDL_Log("SDL 平台信息：%s", SDL_GetPlatform());
}

void sdl_stu::sdl_memory(){
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

void sdl_stu::sdl_atomic(){
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

void sdl_stu::sdl_event(){
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

void sdl_stu::sdl_file_io_module(std::string file_path) {
    // 写入文件
    std::ofstream outfile(file_path);
    if (outfile.is_open()) {
        outfile << "Hello, SDL file I/O!" << std::endl;
        outfile.close();
        std::cout << "Successfully wrote to the file." << std::endl;
    } else {
        std::cerr << "Failed to open the file for writing." << std::endl;
    }

    // 读取文件
    std::ifstream infile(file_path);
    if (infile.is_open()) {
        std::string line;
        while (std::getline(infile, line)) {
            std::cout << "Read from file: " << line << std::endl;
        }
        infile.close();
    } else {
        std::cerr << "Failed to open the file for reading." << std::endl;
    }
}

void sdl_stu::sdl_image_module(std::string image_path) {
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
    SDL_Surface* image_surface = IMG_Load(image_path.c_str());
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

void sdl_stu::sdl_input(){
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

// 线程函数
int thread_function(void* data) {
    const char* thread_name = static_cast<const char*>(data);
    for (int i = 0; i < 5; ++i) {
        std::cout << thread_name << " running: " << i << std::endl;
        SDL_Delay(1000); // 延迟1秒
    }
    return 0;
}

void sdl_stu::sdl_thread_module() {
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

// 计时器回调函数
Uint32 timer_callback(Uint32 interval, void* param) {
    std::cout << "Timer callback called after " << interval << " milliseconds." << std::endl;
    return interval; // 返回值为0表示不重复，返回interval表示重复
}

void sdl_stu::sdl_time_module() {
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

void sdl_stu::sdl_window(){
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

void sdl_stu::render_and_draw(){
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

void sdl_stu::sdl_image(std::string image_path){
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
    SDL_Surface* surface = IMG_Load(image_path.c_str());
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

void sdl_stu::sdl_ttf_module(std::string file_path) {
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
    TTF_Font* font = TTF_OpenFont(file_path.c_str(), 24);
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