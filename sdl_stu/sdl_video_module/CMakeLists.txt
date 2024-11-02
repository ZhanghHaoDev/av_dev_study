add_library(video_module STATIC 
    video_module.cc
)

include_directories(/opt/homebrew/include/)
link_directories(/opt/homebrew/lib)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

target_link_libraries(video_module PUBLIC
    SDL2::SDL2
    SDL2_image::SDL2_image
)