add_library(file_convert STATIC
    file_convert.cc
)

target_link_libraries(file_convert PUBLIC 
    glog::glog
    "/opt/homebrew/Cellar/ffmpeg/7.0.2_1/lib/libavformat.dylib"
    "/opt/homebrew/Cellar/ffmpeg/7.0.2_1/lib/libavcodec.dylib"
    "/opt/homebrew/Cellar/ffmpeg/7.0.2_1/lib/libavutil.dylib"
    "/opt/homebrew/Cellar/ffmpeg/7.0.2_1/lib/libswresample.dylib"
)