cmake_minimum_required(VERSION 3.3)

project(Miwa)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
add_compile_options("-fpermissive")

file(GLOB FSRC
    "*.cxx"
    "miwa/window/*.cxx"
    "miwa/core/ecs/*.cxx"
    "miwa/deps/source/*.c"
    "miwa/graphics/*.cxx"
    "miwa/system/*.cxx"
    "miwa/managers/*.cxx"
    "main/*.cxx"
    "audio/*.cxx"
)

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/miwa/deps/include/)

find_library(glfw REQUIRED)
find_library(dl REQUIRED)
find_library(openal REQUIRED)
find_library(vorbis REQUIRED)
find_library(vorbisfile REQUIRED)
find_library(GL REQUIRED)

add_executable(miwa ${FSRC})
target_link_libraries(miwa dl glfw stdc++fs GL openal vorbis vorbisfile SOIL)
target_compile_definitions(miwa PRIVATE GLFW_INCLUDE_NONE=1)