add_library(audio_module STATIC
    audio_module.cc
)

include_directories(/opt/homebrew/include/)
link_directories(/opt/homebrew/lib)

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

target_link_libraries(audio_module PUBLIC
    SDL2::SDL2
    SDL2_mixer::SDL2_mixer
)
