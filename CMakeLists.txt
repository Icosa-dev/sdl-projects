cmake_minimum_required(VERSION 3.14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(sdl-projects)

include(cmake/CPM.cmake)
CPMAddPackage("gh:libsdl-org/SDL#release-3.4.2")

add_subdirectory("snake")
add_subdirectory("cgl")
add_subdirectory("sv")
add_subdirectory("boids")
