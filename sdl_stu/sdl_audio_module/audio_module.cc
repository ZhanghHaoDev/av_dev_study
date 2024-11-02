#include "audio_module.h"

#include <iostream>

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

audio_module::audio_module(){
    if (SDL_Init(SDL_INIT_AUDIO) < 0){
        std::cout << "SDL 初始化失败！SDL_Error: " << SDL_GetError() << std::endl;
    }
    else{
        is_initialized = true;
        std::cout << "SDL 初始化成功！" << std::endl;
    }
}

audio_module::~audio_module(){
    SDL_Quit();
    is_initialized = false;
}

void audioCallback(void* userdata, Uint8* stream, int len) {
    static double phase = 0.0;
    double frequency = 440.0; // 频率（Hz）
    double sampleRate = 44100.0; // 采样率（Hz）
    double amplitude = 0.5; // 振幅

    // 将音频缓冲区清零
    SDL_memset(stream, 0, len);

    // 填充音频缓冲区
    float* buffer = (float*)stream;
    int samples = len / sizeof(float);
    for (int i = 0; i < samples; ++i) {
        buffer[i] = amplitude * sin(phase * 2.0 * M_PI);
        phase += frequency / sampleRate;
        if (phase >= 1.0) {
            phase -= 1.0;
        }
    }
}

void audio_module::sdl_audio_play() {
    std::string wav_file = "/Users/zhanghao/code/cpp/sdl_stu/assets/test.wav";
    // 初始化SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

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
    desiredSpec.callback = audioCallback; // 音频回调函数

    // 打开音频设备
    if (SDL_OpenAudio(&desiredSpec, &obtainedSpec) < 0) {
        std::cerr << "SDL could not open audio! SDL_Error: " << SDL_GetError() << std::endl;
        Mix_CloseAudio();
        SDL_Quit();
        return;
    }

    // 加载音效
    Mix_Chunk* sound = Mix_LoadWAV(wav_file.c_str());
    if (sound == NULL) {
        std::cerr << "Failed to load sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
        SDL_CloseAudio();
        Mix_CloseAudio();
        SDL_Quit();
        return;
    }

    // 加载音乐
    Mix_Music* music = Mix_LoadMUS(wav_file.c_str());
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

    // 退出SDL
    SDL_Quit();
}