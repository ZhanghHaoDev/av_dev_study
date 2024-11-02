#ifndef CORE_MODULE_H
#define CORE_MODULE_H

class core_model{
public:
    core_model();
    ~core_model();

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

private:
    bool is_initialized = false;
};
#endif // CORE_MODULE_H