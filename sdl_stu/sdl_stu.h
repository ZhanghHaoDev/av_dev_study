#ifndef SDL_STU_H
#define SDL_STU_H

#include <string>

class sdl_stu{
public:
    sdl_stu();
    ~sdl_stu();

    // 关于播放音频的函数
    void sdl_audio_play(std::string file_path);

    // 关于SDL的版本的函数
    void get_sdl_version();

    // 关于SDL的日志的函数
    void sdl_log();

    // 关于SDL错误处理的函数
    void sdl_error();

    // 关于SDL平台信息的函数
    void sdl_platform();

    // 关于SDL内存操作的函数
    void sdl_memory();

    // 关于SDL的原子操作的函数
    void sdl_atomic();

    // SDL 事件函数
    void sdl_event();
    
    // SDL 文件io 函数
    void sdl_file_io_module(std::string file_path);

    // SDL 图片 函数
    void sdl_image_module(std::string image_path);

    // sdl 键盘输入 函数
    void sdl_input();

    // sdl 线程函数
    void sdl_thread_module();

    // sdl 时间函数
    void sdl_time_module();

    // 关于窗口的函数
    void sdl_window();

    // 关于渲染和绘制的函数
    void render_and_draw();

    // 关于图像操作的函数
    void sdl_image(std::string file_path);

    // sdl 字体函数
    void sdl_ttf_module(std::string file_path);

};

#endif // SDL_STU_H