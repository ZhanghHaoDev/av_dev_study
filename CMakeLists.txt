cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(av_dev_study)

set(CMAKE_BUILD_TYPE Release)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_subdirectory(ffmpeg_stu)
#add_subdirectory(opengl_stu)
add_subdirectory(sdl_stu)
add_subdirectory(test)
add_subdirectory(h264_info)
#add_subdirectory(webrtc_stu)