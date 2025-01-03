# SPDX-License-Identifier: BSD-3-Clause
# cmake version 3.20
# list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
# find_package(FFmpeg REQUIRED)
# target_link_libraries(project_name PRIVATE FFmpeg::FFmpeg)
# target_link_libraries(project_name PRIVATE FFmpeg::avcodec FFmpeg::avformat FFmpeg::avutil FFmpeg::swscale)

include(FindPackageHandleStandardArgs)
set(FFmpeg_FIND_COMPONENTS AVCODEC AVFORMAT AVUTIL SWSCALE SWRESAMPLE AVFILTER POSTPROC)

set(FFMPEG_USER_DIR CACHE PATH "FFmpeg USER directory")
if(UNIX AND NOT APPLE)
	set(FFMPEG_POSSIBLE_ROOT_DIRS
		${FFMPEG_USER_DIR}
		/usr/local
		/usr
		/opt
	)
elseif(APPLE)
	set(FFMPEG_POSSIBLE_ROOT_DIRS
		${FFMPEG_USER_DIR}
		/opt/homebrew/opt/ffmpeg
		/usr/local/opt/ffmpeg
		/usr/local
		/usr
	)
elseif(WIN32)
	set(FFMPEG_POSSIBLE_ROOT_DIRS
		${FFMPEG_USER_DIR}
		C:/ffmpeg
		"C:/Program Files/ffmpeg"
		"C:/Program Files (x86)/ffmpeg"
	)
endif()

foreach(FFMPEG_DIR ${FFMPEG_POSSIBLE_ROOT_DIRS})
	if(EXISTS "${FFMPEG_DIR}/include/libavformat/avformat.h")
		set(FFMPEG_INCLUDE_DIRS "${FFMPEG_DIR}/include")
		set(FFMPEG_LIBRARY_DIRS "${FFMPEG_DIR}/lib")
		break()
	endif()
endforeach()

foreach(_component ${FFmpeg_FIND_COMPONENTS})
	string(TOLOWER ${_component} _lower)
	find_path(${_component}_INCLUDE_DIR
		NAMES lib${_lower}/${_lower}.h
		HINTS ${FFMPEG_INCLUDE_DIRS}
		NO_DEFAULT_PATH
	)
	find_library(${_component}_LIBRARY
		NAMES ${_lower}
		HINTS ${FFMPEG_LIBRARY_DIRS}
		NO_DEFAULT_PATH
	)
	if(${_component}_INCLUDE_DIR AND ${_component}_LIBRARY)
		set(${_component}_FOUND TRUE)
		list(APPEND FFMPEG_INCLUDE_DIRS ${${_component}_INCLUDE_DIR})
		list(APPEND FFMPEG_LIBRARIES ${${_component}_LIBRARY})
	else()
		set(${_component}_FOUND FALSE)
	endif()
endforeach()

foreach(_component ${FFmpeg_FIND_COMPONENTS})
	if(${_component}_FOUND)
		add_library(FFmpeg::${_component} INTERFACE IMPORTED)
		set_target_properties(FFmpeg::${_component} PROPERTIES
			INTERFACE_INCLUDE_DIRECTORIES "${${_component}_INCLUDE_DIR}"
			INTERFACE_LINK_LIBRARIES "${${_component}_LIBRARY}"
		)
	endif()
endforeach()

if(AVCODEC_FOUND AND AVFORMAT_FOUND AND AVUTIL_FOUND AND SWSCALE_FOUND)
	add_library(FFmpeg::FFmpeg INTERFACE IMPORTED)
	set_target_properties(FFmpeg::FFmpeg PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES "${FFMPEG_INCLUDE_DIRS}"
		INTERFACE_LINK_LIBRARIES "${FFMPEG_LIBRARIES}"
	)
endif()

set(_FFmpeg_REQUIRED_VARS
    AVCODEC_LIBRARY
    AVFORMAT_LIBRARY
    AVUTIL_LIBRARY
    SWSCALE_LIBRARY
    SWRESAMPLE_LIBRARY
    AVFILTER_LIBRARY
    POSTPROC_LIBRARY
    FFMPEG_INCLUDE_DIRS
)

find_package_handle_standard_args(FFmpeg
	REQUIRED_VARS ${_FFmpeg_REQUIRED_VARS}
	FAIL_MESSAGE "FFmpeg libraries or headers not found. Please install FFmpeg or set FFMPEG_USER_DIR."
)