include_directories(
    ${CMAKE_SOURCE_DIR}/qt_interface
    ${CMAKE_SOURCE_DIR}/file_info
)

add_executable(media_fusion_app
    main.cc
)

find_package(fmt REQUIRED)

target_link_libraries(media_fusion_app PUBLIC
    fmt::fmt
    qt_interface
    file_info
)