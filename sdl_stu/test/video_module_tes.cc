#include <iostream>

#include "video_module.h"

#include "gtest/gtest.h"
#include <iterator>

TEST(video_module,sdl_window_tes){
    video_module module;
    module.sdl_window();
}

TEST(video_module, render_and_draw_test){
    video_module module;
    module.render_and_draw();
}

TEST(video_module, sdl_image_test){
    std::cout << "ss" << std::endl;
    video_module module;
    module.sdl_image();
}
