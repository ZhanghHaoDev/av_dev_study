set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# add_executable(opengl_stu_test
#     opengl_stu_test.cpp
# )

# target_include_directories(opengl_stu_test PUBLIC
#     ${CMAKE_SOURCE_DIR}/opengl_stu
# )

# target_link_libraries(opengl_stu_test
#     opengl_stu
# )

################################################################################
add_executable(sdl_stu_test
    sdl_stu_test.cpp
)

target_include_directories(sdl_stu_test PUBLIC
    ${CMAKE_SOURCE_DIR}/sdl_stu
)

target_link_libraries(sdl_stu_test
    sdl_stu
)

################################################################################

add_executable(ffmpeg_stu_test
    ffmpeg_stu_test.cpp
)

target_include_directories(ffmpeg_stu_test PUBLIC
    ${CMAKE_SOURCE_DIR}/ffmpeg_stu
)

target_link_libraries(ffmpeg_stu_test PUBLIC
    ffmpeg_stu
)

################################################################################

add_executable(h264_info_test
    h264_info_test.cpp
)

target_include_directories(h264_info_test PRIVATE
    ${CMAKE_SOURCE_DIR}/h264_info
    ${CMAKE_SOURCE_DIR}/ffmpeg_stu
)

find_package(Qt6 REQUIRED COMPONENTS Widgets MultimediaWidgets Quick QuickWidgets)

target_link_libraries(h264_info_test PRIVATE
    h264_info
    Qt6::Widgets
    Qt6::MultimediaWidgets
    Qt6::Quick
    Qt6::QuickWidgets
    ffmpeg_stu
)

################################################################################

# add_executable(webrtc_stu_text
#     webrtc_stu_text.cpp
# )

# target_include_directories(webrtc_stu_text PRIVATE
#     ${CMAKE_SOURCE_DIR}/webrtc_stu
# )

# target_link_libraries(webrtc_stu_text PRIVATE
#     webrtc_stu
# )