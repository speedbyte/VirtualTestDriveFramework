cmake_minimum_required(VERSION 3.5)
project(SaliencySandbox2_0)

cmake_policy(SET CMP0015 NEW)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(Protobuf REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED filesystem regex serialization unit_test_framework iostreams program_options thread)
find_package(Graphviz REQUIRED)

include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_BINARY_DIR})
include_directories(include)


install(DIRECTORY include/ DESTINATION include/ FILES_MATCHING PATTERN "*.h")
install(DIRECTORY data/ DESTINATION data/)
install(DIRECTORY config/ DESTINATION config/)
install(DIRECTORY scripts/ DESTINATION scripts/)

add_subdirectory(src/core/)
add_subdirectory(src/utils/)
add_subdirectory(src/gaze/)
add_subdirectory(src/eyetribe/)
add_subdirectory(src/io/)
add_subdirectory(src/saliency/)
add_subdirectory(src/plot/)
add_subdirectory(src/kitti/)
add_subdirectory(src/tools/)
add_subdirectory(src/test/)
