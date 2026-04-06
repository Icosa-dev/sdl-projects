set(SUBPROJECT_NAME boids)
file(GLOB_RECURSE SOURCES "*.c")

find_library(MATH_LIBRARY m)

add_executable(${SUBPROJECT_NAME} ${SOURCES})
target_link_libraries(${SUBPROJECT_NAME} PRIVATE SDL3::SDL3 ${MATH_LIBRARY})
target_include_directories(${SUBPROJECT_NAME} PRIVATE "src/")
