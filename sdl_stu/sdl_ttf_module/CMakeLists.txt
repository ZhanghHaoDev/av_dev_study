add_library(ttf_module STATIC 
    ttf_module.cc
)

include_directories(/opt/homebrew/include/)
link_directories(/opt/homebrew/lib)
include_directories(/opt/homebrew/include/SDL2)
link_directories(/opt/homebrew/opt/sdl2_ttf/lib)

find_package(SDL2 REQUIRED)

target_link_libraries(ttf_module PUBLIC
    SDL2::SDL2
    SDL2_ttf
)