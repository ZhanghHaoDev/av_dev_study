# FindFFmpeg.cmake 
#
# 如何使用？
# # 查找 FFmpeg 库
# list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
# find_package(FFmpeg REQUIRED)
# target_link_libraries(project_name PRIVATE FFmpeg::FFmpeg)

set(FFMPEG_ROOT_DIR                    # 自定义 FFMPEG 的位置
    CACHE PATH "FFmpeg root directory"
)

if(UNIX AND NOT APPLE)                  
    set(FFMPEG_POSSIBLE_ROOT_DIRS       # Linux 系统
        ${FFMPEG_ROOT_DIR}
        /usr/local
        /usr
        /opt
    )
elseif(APPLE)
    set(FFMPEG_POSSIBLE_ROOT_DIRS       # macOS 系统
        ${FFMPEG_ROOT_DIR}
        /opt/homebrew/opt/ffmpeg       
        /usr/local/opt/ffmpeg           
        /usr/local                      
        /usr                            
    )
elseif(WIN32)
    set(FFMPEG_POSSIBLE_ROOT_DIRS       # Windows 系统
        ${FFMPEG_ROOT_DIR}
        C:/ffmpeg                       
        "C:/Program Files/ffmpeg"       
        "C:/Program Files (x86)/ffmpeg" 
    )
endif()

set(FFMPEG_FOUND FALSE)                 # 查找 FFmpeg 库和头文件
foreach(FFMPEG_DIR ${FFMPEG_POSSIBLE_ROOT_DIRS})
    message(STATUS "正在检查 FFmpeg 路径：${FFMPEG_DIR}")
    if(EXISTS "${FFMPEG_DIR}/include/libavformat/avformat.h")
        set(FFMPEG_INCLUDE_DIRS "${FFMPEG_DIR}/include")
        set(FFMPEG_LIBRARY_DIRS "${FFMPEG_DIR}/lib")
        find_library(AVFORMAT_LIBRARY avformat HINTS ${FFMPEG_LIBRARY_DIRS})
        find_library(AVCODEC_LIBRARY avcodec HINTS ${FFMPEG_LIBRARY_DIRS})
        find_library(AVUTIL_LIBRARY avutil HINTS ${FFMPEG_LIBRARY_DIRS})
        find_library(SWSCALE_LIBRARY swscale HINTS ${FFMPEG_LIBRARY_DIRS})

        set(FFMPEG_LIBRARIES
            ${AVFORMAT_LIBRARY}
            ${AVCODEC_LIBRARY}
            ${AVUTIL_LIBRARY}
            ${SWSCALE_LIBRARY}
        )
        if(AVFORMAT_LIBRARY AND AVCODEC_LIBRARY AND AVUTIL_LIBRARY AND SWSCALE_LIBRARY)
            set(FFMPEG_FOUND TRUE)
            message(STATUS "在 ${FFMPEG_DIR} 找到了 FFmpeg")
            set(FFMPEG_INCLUDE_DIRS ${FFMPEG_INCLUDE_DIRS})
            set(FFMPEG_LIBRARIES ${FFMPEG_LIBRARIES})
            
            execute_process(           
                COMMAND ${FFMPEG_DIR}/bin/ffmpeg -version
                OUTPUT_VARIABLE FFMPEG_VERSION_INFO
                OUTPUT_STRIP_TRAILING_WHITESPACE
            )
            message(STATUS "FFmpeg version: ${FFMPEG_VERSION_INFO}")
            break()
        else()
            message(STATUS "在 ${FFMPEG_DIR} 中未找到所有必需的 FFmpeg 库")
        endif()
    else()
        message(STATUS "在 ${FFMPEG_DIR} 中未找到 FFmpeg 头文件")
    endif()
endforeach()

if(FFMPEG_FOUND)                        # 设置导入目标的属性，指定头文件目录和库
    message(STATUS "在 ${FFMPEG_DIR} 找到了 FFmpeg")
    add_library(FFmpeg::FFmpeg INTERFACE IMPORTED) 
    set_target_properties(FFmpeg::FFmpeg PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${FFMPEG_INCLUDE_DIRS}"
        INTERFACE_LINK_LIBRARIES "${FFMPEG_LIBRARIES}"
    )
else()
    message(FATAL_ERROR "未找到 FFmpeg，请安装 FFmpeg 或指定正确的 FFMPEG_ROOT_DIR")
endif()