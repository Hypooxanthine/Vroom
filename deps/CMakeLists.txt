cmake_minimum_required (VERSION 3.31)

set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)
add_subdirectory(glew)