add_library(core_module STATIC 
    core_module.cc
)

include_directories(/opt/homebrew/include/)
link_directories(/opt/homebrew/lib)

find_package(SDL2 REQUIRED)

target_link_libraries(core_module PUBLIC
    SDL2::SDL2
)