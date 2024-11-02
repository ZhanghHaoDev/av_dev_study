add_library(timer_module STATIC 
    timer_module.cc
)

include_directories(/opt/homebrew/include/)
link_directories(/opt/homebrew/lib)

find_package(SDL2 REQUIRED)

target_link_libraries(timer_module PUBLIC
    SDL2::SDL2
)