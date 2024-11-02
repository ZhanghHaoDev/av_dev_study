include_directories(
    "../sdl_core_module"
    "../sdl_video_module"
    "../sdl_audio_module"
    "../sdl_event_module"
    "../sdl_input_module/"
    "../sdl_thread_module"
    "../sdl_timer_module/"
    "../sdl_file_io_module"
    "../sdl_image_module"
    "../sdl_ttf_module"
)

include_directories(/opt/homebrew/include/SDL2)
link_directories(/opt/homebrew/lib /opt/homebrew/opt/sdl2_ttf/lib)

# 查找 SDL2 和 SDL2_ttf 包
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(sdl_module_test
    main.cc
    core_module_test.cc
    video_module_tes.cc
    audio_module_test.cc
    event_module_test.cc
    input_module_test.cc
    thread_module_test.cc
    timer_module_test.cc
    file_io_module_test.cc
    image_module_test.cc
    ttf_module_test.cc
)

find_package(GTest REQUIRED)

target_link_libraries(sdl_module_test PUBLIC
    GTest::GTest
    GTest::Main
    core_module  
    video_module  
    audio_module 
    event_module
    input_module
    thread_module
    timer_module
    file_io_module
    image_module
    ttf_module

    SDL2::SDL2
    SDL2_ttf::SDL2_ttf
)

enable_testing()
add_test(NAME sdl_module_test COMMAND sdl_module_test)