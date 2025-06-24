cmake_minimum_required (VERSION 3.31)

# ----- CMake settings -----

# Policies
if (POLICY CMP0072)
    cmake_policy (SET CMP0072 NEW)
endif(POLICY CMP0072)

# ----- General compiler settings -----

# C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Warnings
# if (UNIX)
#     add_compile_options(-Wno-volatile)
# endif(UNIX)

# Preprocessor defines
if (WIN32)
    add_compile_definitions(VRM_PLATFORM_WINDOWS=1)
endif()

if (UNIX)
    add_compile_definitions(VRM_PLATFORM_LINUX=1)
endif()

add_compile_definitions(
  $<$<CONFIG:Debug>:VRM_DEBUG>
  $<$<CONFIG:Release>:VRM_RELEASE>
)

# Enable glm experimental features for all projects
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL=1)

# Visual Studio specific settings
if (CMAKE_GENERATOR MATCHES "Visual Studio")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT VroomEditor)
endif()

# ----- Project general definitions -----

# Root directory
get_filename_component(ROOT "${CMAKE_SOURCE_DIR}" ABSOLUTE)

# Resource directories
set(ENGINE_RESOURCE_DIR   ${ROOT}/Vroom/Resources)
set(EDITOR_RESOURCE_DIR   ${ROOT}/VroomEditor/Resources)

# ----- Solution definition -----

project ("VroomSolution")

# ----- Subdmodules -----

add_subdirectory("deps")

# ----- Subdirectories -----

add_subdirectory ("Vroom")
add_subdirectory("Sandbox")
add_subdirectory("VroomEditor")

# ----- Testing -----

include(CTest)