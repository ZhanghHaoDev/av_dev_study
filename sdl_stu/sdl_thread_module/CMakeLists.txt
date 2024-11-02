add_library(thread_module STATIC 
    thread_module.cc
)

include_directories(/opt/homebrew/include/)
link_directories(/opt/homebrew/lib)

find_package(SDL2 REQUIRED)

target_link_libraries(thread_module PUBLIC
    SDL2::SDL2
)