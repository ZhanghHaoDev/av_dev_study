#ifndef VIDEO_MODULE_H
#define VIDEO_MODULE_H

class video_module{
public:
    video_module();
    ~video_module();

    // 关于窗口的函数
    void sdl_window();

    // 关于渲染和绘制的函数
    /*
    * 1. 创建渲染器
    * 2. 设置渲染颜色
    * 3. 清除渲染目标
    * 4. 显示渲染结果
    * 5. 销毁渲染器
    * 6. 绘制矩形
    * 7. 填充矩形
    * 8. 绘制线条
    * 9. 绘制点
    * 10. 绘制纹理
    */
    void render_and_draw();

    // 关于图像操作的函数
    /*
    * 1. 加载图像
    * 2. 创建纹理
    * 3. 释放表面
    * 4. 销毁纹理
    * 5. 图像格式转换
    * 6. 图像缩放
    * 7. 图像旋转
    */
    void sdl_image();

private:
    bool is_initialized;
};

#endif // VIDEO_MODULE_H