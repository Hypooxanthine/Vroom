cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0072)
    cmake_policy (SET CMP0072 NEW)
endif(POLICY CMP0072)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

get_filename_component(ROOT "${CMAKE_SOURCE_DIR}" ABSOLUTE)
set(DEPENDENCIES ${ROOT}/dependencies/)

find_package(OpenGL REQUIRED)

# Dependencies include directories
set(GLM_INCLUDE_DIR ${DEPENDENCIES}/GLM/include)
set(STB_IMAGE_INCLUDE_DIR ${DEPENDENCIES}/stb_image/include)
set(SPDLOG_INCLUDE_DIR ${DEPENDENCIES}/spdlog/include)
if(WIN32)
    set(GLEW_INCLUDE_DIR ${DEPENDENCIES}/GLEW/include)
    set(GLFW_INCLUDE_DIR ${DEPENDENCIES}/GLFW/include)
elseif(UNIX)
    set(GLEW_INCLUDE_DIR )
    set(GLFW_INCLUDE_DIR )
endif(WIN32)

set(LIBS_INCLUDES
    ${INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${STB_IMAGE_INCLUDE_DIR}
    ${SPDLOG_INCLUDE_DIR}
)

# Dependencies lib directories
if(WIN32)
    set(GLEW_LIBRARY ${DEPENDENCIES}/GLEW/lib-vc2022/glew32s.lib opengl32)
    set(GLFW_LIBRARY ${DEPENDENCIES}/GLFW/lib-vc2022/glfw3.lib)
elseif(UNIX)
    set(GLFW_LIBRARY glfw3 X11)
    set(GLEW_LIBRARY GLEW)
endif(WIN32)

project ("Vroom")

add_subdirectory ("Vroom")
set(VROOM ${ROOT}/Vroom/)
add_subdirectory("Sandbox")