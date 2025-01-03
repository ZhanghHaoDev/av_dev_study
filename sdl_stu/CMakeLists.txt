add_library(sdl_stu OBJECT
    sdl_stu.cpp
)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

target_link_libraries(sdl_stu PRIVATE
    SDL2::SDL2 
    SDL2::SDL2_image 
    SDL2::SDL2_mixer 
    SDL2::SDL2_ttf
)