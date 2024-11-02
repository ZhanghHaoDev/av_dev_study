#ifndef AUDIO_MODULE_H
#define AUDIO_MODULE_H

class audio_module{
public:
    audio_module();
    ~audio_module();

    // 关于播放音频的函数
    void sdl_audio_play();

private:
    bool is_initialized;

};

#endif // AUDIO_MODULE_H