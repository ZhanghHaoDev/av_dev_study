set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(
    ${CMAKE_SOURCE_DIR}/file_info
)

add_library(qt_interface STATIC
)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

target_link_libraries(qt_interface PUBLIC 
    glog::glog
    Qt6::Widgets
    file_info
)