cmake_minimum_required(VERSION 3.5)
project(SaliencySandbox2_0)

cmake_policy(SET CMP0015 NEW)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install")
#Native external library installation through apt-get
find_package(Protobuf REQUIRED)
#find_package(OpenCV REQUIRED)
#find_package(Boost REQUIRED filesystem regex serialization unit_test_framework iostreams program_options thread)
find_package(Graphviz REQUIRED)

find_package(PkgConfig REQUIRED)
set(ENV{PKG_CONFIG_PATH}  "${CMAKE_SOURCE_DIR}/../../libs/opencv-install/lib/pkgconfig")
set(ENV{PKG_CONFIG_PATH}  "$ENV{PKG_CONFIG_PATH}:${CMAKE_SOURCE_DIR}/../../libs/boost-install/lib/pkgconfig")
set(ENV{PKG_CONFIG_PATH}  "$ENV{PKG_CONFIG_PATH}:${CMAKE_SOURCE_DIR}/../../libs/ffmpeg-install/lib/pkgconfig")

pkg_search_module(pkg_boost REQUIRED boost)
pkg_search_module(pkg_opencv REQUIRED opencv)
pkg_search_module(pkg_ffmpeg_libavcodec REQUIRED libavcodec)
pkg_search_module(pkg_ffmpeg_libswresample REQUIRED libswresample)

#find_library(FFMPEG_libavcodec NAMES libavcodec.so PATHS "${CMAKE_SOURCE_DIR}/../../libs/ffmpeg-install/lib/")
#find_library(FFMPEG_libswresample NAMES libswresample.so HINTS "${CMAKE_SOURCE_DIR}/../../libs/ffmpeg-install/lib/")

MESSAGE("       headers_boost: ${pkg_boost_INCLUDE_DIRS}")
MESSAGE("       headers_opencv: ${pkg_opencv_INCLUDE_DIRS}")
MESSAGE("       headers_ffmpeg: ${pkg_ffmpeg_libavcodec_INCLUDE_DIRS}")
MESSAGE("       libraries_boost: ${pkg_boost_LDFLAGS}")
MESSAGE("       libraries_opencv: ${pkg_opencv_LDFLAGS}")
MESSAGE("       libraries_ffmpeg_libavcodec: ${pkg_ffmpeg_libavcodec_LDFLAGS}")

include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_BINARY_DIR})
include_directories(${pkg_boost_INCLUDE_DIRS})
include_directories(${pkg_opencv_INCLUDE_DIRS})
include_directories(${pkg_ffmpeg_libavcodec_INCLUDE_DIRS})
include_directories(${pkg_ffmpeg_libswresample_INCLUDE_DIRS})
include_directories("${CMAKE_SOURCE_DIR}/../../utils/gnuplot-iostream")
include_directories(include)

install(DIRECTORY include/ DESTINATION include/ FILES_MATCHING PATTERN "*.h")
#install(DIRECTORY data/ DESTINATION data/)
#install(DIRECTORY config/ DESTINATION config/)
#install(DIRECTORY scripts/ DESTINATION scripts/)

set(EXTERNAL_LIBS "${pkg_opencv_LDFLAGS} ${pkg_boost_LDFLAGS} ${pkg_ffmpeg_libswresample_LDFLAGS} ${pkg_ffmpeg_libavcodec_LDFLAGS}")

add_subdirectory(src/core/)
add_subdirectory(src/utils/)
add_subdirectory(src/gaze/)
add_subdirectory(src/eyetribe/)
add_subdirectory(src/io/)
add_subdirectory(src/saliency/)
add_subdirectory(src/plot/)
add_subdirectory(src/kitti/)
add_subdirectory(src/tracking/)
add_subdirectory(src/adtf/)
add_subdirectory(src/tools/)
add_subdirectory(src/test/)
add_subdirectory(src/run_configurations)