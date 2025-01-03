# SPDX-License-Identifier: BSD-3-Clause
# cmake version 3.20
# list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
# find_package(SDL2 REQUIRED)
# find_package(SDL2_image REQUIRED)
# find_package(SDL2_mixer REQUIRED)
# find_package(SDL2_ttf REQUIRED)
# target_link_libraries(project_name PRIVATE SDL2::SDL2 SDL2::SDL2_mixer SDL2::SDL2_image SDL2::SDL2_ttf)
# 使用头文件，不需要添加SDL2/ 前缀，直接引用SDL.h

include(FindPackageHandleStandardArgs)
set(SDL2_FIND_COMPONENTS SDL2 SDL2_image SDL2_mixer SDL2_ttf CACHE STRING "SDL2 components to find")

set(SDL2_USER_PATH CACHE PATH "SDL2 USER directory")

if(UNIX AND NOT APPLE)
    set(SDL2_POSSIBLE_PATH
        ${SDL2_USER_PATH}
        /usr/local
        /usr
    )
elseif(APPLE)
    set(SDL2_POSSIBLE_PATH
        ${SDL2_USER_PATH}
        /opt/homebrew/opt/sdl2
        /opt/homebrew/opt/sdl2_mixer
        /opt/homebrew/opt/sdl2_image
        /opt/homebrew/opt/sdl2_ttf
    )
elseif(WIN32)
    set(SDL2_POSSIBLE_PATH
        ${SDL2_USER_PATH}
    )
endif()

foreach(SDL2_DIR ${SDL2_POSSIBLE_PATH})
    foreach(COMPONENT ${SDL2_FIND_COMPONENTS})
        if(COMPONENT STREQUAL "SDL2")
            set(COMPONENT_HEADER "SDL.h")
            set(COMPONENT_LIBRARY "libSDL2.dylib")
        elseif(COMPONENT STREQUAL "SDL2_image")
            set(COMPONENT_HEADER "SDL_image.h")
            set(COMPONENT_LIBRARY "libSDL2_image.dylib")
        elseif(COMPONENT STREQUAL "SDL2_mixer")
            set(COMPONENT_HEADER "SDL_mixer.h")
            set(COMPONENT_LIBRARY "libSDL2_mixer.dylib")
        elseif(COMPONENT STREQUAL "SDL2_ttf")
            set(COMPONENT_HEADER "SDL_ttf.h")
            set(COMPONENT_LIBRARY "libSDL2_ttf.dylib")
        endif()

        if(EXISTS "${SDL2_DIR}/include/SDL2/${COMPONENT_HEADER}")
            list(APPEND SDL2_INCLUDE_DIRS "${SDL2_DIR}/include/SDL2")
            list(APPEND SDL2_LIBRARY_DIRS "${SDL2_DIR}/lib")
            list(APPEND SDL2_LIBRARIES "${SDL2_DIR}/lib/${COMPONENT_LIBRARY}")
        endif()
    endforeach()
endforeach()

find_package_handle_standard_args(SDL2 REQUIRED_VARS SDL2_INCLUDE_DIRS SDL2_LIBRARY_DIRS SDL2_LIBRARIES)

if(SDL2_FOUND)
    if(NOT TARGET SDL2::SDL2)
        add_library(SDL2::SDL2 INTERFACE IMPORTED)
        set_target_properties(SDL2::SDL2 PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIRS}"
            INTERFACE_LINK_LIBRARIES "${SDL2_LIBRARIES}"
        )
    endif()

    if(NOT TARGET SDL2::SDL2_image)
        add_library(SDL2::SDL2_image INTERFACE IMPORTED)
        set_target_properties(SDL2::SDL2_image PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIRS}"
            INTERFACE_LINK_LIBRARIES "${SDL2_LIBRARIES}"
        )
    endif()

    if(NOT TARGET SDL2::SDL2_mixer)
        add_library(SDL2::SDL2_mixer INTERFACE IMPORTED)
        set_target_properties(SDL2::SDL2_mixer PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIRS}"
            INTERFACE_LINK_LIBRARIES "${SDL2_LIBRARIES}"
        )
    endif()

    if(NOT TARGET SDL2::SDL2_ttf)
        add_library(SDL2::SDL2_ttf INTERFACE IMPORTED)
        set_target_properties(SDL2::SDL2_ttf PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIRS}"
            INTERFACE_LINK_LIBRARIES "${SDL2_LIBRARIES}"
        )
    endif()
else()
    set(SDL2_FOUND FALSE)
    set(SDL2_IMAGE_FOUND FALSE)
    set(SDL2_MIXER_FOUND FALSE)
    set(SDL2_TTF_FOUND FALSE)
endif()