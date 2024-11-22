add_library(sdl_stu STATIC
    sdl_stu.cpp
)

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(/opt/homebrew/Cellar/sdl2_mixer/2.8.0/include)
include_directories(/opt/homebrew/Cellar/sdl2_image/2.8.2_2/include)
include_directories(/opt/homebrew/Cellar/sdl2_ttf/2.22.0/include)

target_link_libraries(sdl_stu PUBLIC
    SDL2::SDL2
    SDL2_mixer::SDL2_mixer
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
)

