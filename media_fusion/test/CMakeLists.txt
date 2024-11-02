include_directories(
    ${CMAKE_SOURCE_DIR}/file_info
    ${CMAKE_SOURCE_DIR}/file_convert
)

enable_testing()

add_executable(media_fusion_test
    "main_test.cc"
    "./file_info_test.cc"
    "./file_convert_test.cc"
)

target_link_libraries(media_fusion_test PUBLIC  
    gtest_main
    file_info
    file_convert
)

include(GoogleTest)
gtest_discover_tests(media_fusion_test)