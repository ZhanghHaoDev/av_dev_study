add_library(ffmpeg_stu OBJECT
    ffmpeg_stu.cpp
)

find_package(FFmpeg REQUIRED)

target_link_libraries(ffmpeg_stu PRIVATE 
    FFmpeg::FFmpeg
)