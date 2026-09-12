cmake_minimum_required(VERSION 4.3)
project(software_rasterizer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

include(FetchContent)

FetchContent_Declare(
	SDL3
	GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
	GIT_TAG        release-3.4.16
)

FetchContent_MakeAvailable(SDL3)

add_executable(software_rasterizer src/main.cpp)
target_link_libraries(software_rasterizer PRIVATE SDL3::SDL3)
