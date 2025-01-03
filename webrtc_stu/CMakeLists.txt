add_library(webrtc_stu OBJECT
    webrtc_stu.cpp
)

include_directories(
    /Users/zhanghao/code/cpp/webrtc/src
    /Users/zhanghao/code/cpp/webrtc/src/third_party/abseil-cpp
)
link_directories(/Users/zhanghao/code/cpp/webrtc/src/out/Default/obj)

target_link_libraries(webrtc_stu PRIVATE 
    webrtc
)