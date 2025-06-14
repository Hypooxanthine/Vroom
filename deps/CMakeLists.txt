cmake_minimum_required (VERSION 3.31)

set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
set(GLFW_LIBRARY_TYPE "SHARED")
add_subdirectory(glfw)
add_subdirectory(glew)