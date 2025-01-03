set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_library(h264_info OBJECT
    h264_info.cpp
)

find_package(Qt6 REQUIRED COMPONENTS Widgets MultimediaWidgets Quick QuickWidgets)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../ffmpeg_stu)

target_link_libraries(h264_info PRIVATE
    Qt6::Widgets
    Qt6::MultimediaWidgets
    Qt6::Quick
    Qt6::QuickWidgets
    ffmpeg_stu
)