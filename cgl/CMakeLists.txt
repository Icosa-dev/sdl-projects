set(SUBPROJECT_NAME cgl)
file(GLOB_RECURSE SOURCES "*.c")

add_executable(${SUBPROJECT_NAME} ${SOURCES})
target_link_libraries(${SUBPROJECT_NAME} PRIVATE SDL3::SDL3)
target_include_directories(${SUBPROJECT_NAME} PRIVATE "src/")
