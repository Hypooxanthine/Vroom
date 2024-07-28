cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0072)
    cmake_policy (SET CMP0072 NEW)
endif(POLICY CMP0072)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

get_filename_component(ROOT "${CMAKE_SOURCE_DIR}" ABSOLUTE)
set(DEPENDENCIES ${ROOT}/dependencies/)

if (UNIX)
    add_compile_options(-Wno-volatile)
endif(UNIX)

if (WIN32)
    find_package(OpenGL REQUIRED)
endif(WIN32)

# Preprocessor defines

if (VRM_PLATFORM_WINDOWS)
    add_compile_definitions(VRM_PLATFORM_WINDOWS=1)
endif()

if (VRM_PLATFORM_LINUX)
    add_compile_definitions(VRM_PLATFORM_LINUX=1)
endif()

if (VRM_DEBUG)
    add_compile_definitions(VRM_DEBUG=1)
endif()

if (VRM_RELEASE)
    add_compile_definitions(VRM_RELEASE=1)
endif()

add_compile_definitions(_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING=1)


# Dependencies include directories
set(GLM_INCLUDE_DIR ${DEPENDENCIES}/GLM/include)
set(STB_IMAGE_INCLUDE_DIR ${DEPENDENCIES}/stb_image/include)
set(SPDLOG_INCLUDE_DIR ${DEPENDENCIES}/spdlog/include)
set(OBJ_LOADER_INCLUDE_DIR ${DEPENDENCIES}/OBJ_Loader/include)
set(ENTT_INCLUDE_DIR ${DEPENDENCIES}/entt/include)
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
    ${OBJ_LOADER_INCLUDE_DIR}
    ${ENTT_INCLUDE_DIR}
)

# Dependencies lib directories
if(WIN32)
    set(GLEW_LIBRARY ${DEPENDENCIES}/GLEW/lib-vc2022/glew32s.lib opengl32)
    set(GLFW_LIBRARY ${DEPENDENCIES}/GLFW/lib-vc2022/glfw3.lib)
elseif(UNIX)
    set(GLFW_LIBRARY glfw X11)
    set(GLEW_LIBRARY GLEW)
endif(WIN32)

project ("Vroom")

add_subdirectory ("Vroom")
set(VROOM ${ROOT}/Vroom/)
set(RESOURCES ${ROOT}/Resources/)

add_subdirectory("Sandbox")

# Make the Sandbox project the startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sandbox)

include(CTest)

# Compile options
if (MSVC)
    target_compile_options(Vroom PRIVATE /MP)
    target_compile_options(Sandbox PRIVATE /MP)
endif()