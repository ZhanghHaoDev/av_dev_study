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
