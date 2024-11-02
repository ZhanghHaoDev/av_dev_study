add_library(event_module STATIC 
    event_module.cc
)

include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

find_package(SDL2 REQUIRED)

target_link_libraries(event_module PUBLIC 
    SDL2::SDL2
)
