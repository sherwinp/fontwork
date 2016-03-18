cmake_minimum_required (VERSION 2.8)
set(NAME_BINARY "fontwork" CACHE STRING "description")
project (${NAME_BINARY})

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

add_definitions(-g -std=c++11)

file(GLOB SOURCES "source/*.cpp")

SET(CMAKE_EXE_LINKER_FLAGS "-v")

add_executable(${NAME_BINARY} ${SOURCES})
target_link_libraries(${NAME_BINARY} SOIL freetype GL GLEW glfw)
