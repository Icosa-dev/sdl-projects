file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(snake ${SOURCES})
target_link_libraries(snake PRIVATE SDL3::SDL3)
target_include_directories(snake PRIVATE "src/")
