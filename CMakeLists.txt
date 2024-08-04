cmake_minimum_required (VERSION 3.8)

# ----- CMake settings -----

# Policies
if (POLICY CMP0072)
    cmake_policy (SET CMP0072 NEW)
endif(POLICY CMP0072)

# ----- General compiler settings -----

# C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Warnings
if (UNIX)
    add_compile_options(-Wno-volatile)
endif(UNIX)

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

# Enable glm experimental features for all projects
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL=1)

# Visual Studio specific settings
if (CMAKE_GENERATOR MATCHES "Visual Studio")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sandbox)
endif()

# ----- Project general definitions -----

# Root directory
get_filename_component(ROOT "${CMAKE_SOURCE_DIR}" ABSOLUTE)

# ----- Solution definition -----

project ("VroomSolution")

# ----- Subdirectories -----

add_subdirectory ("Vroom")
add_subdirectory("Sandbox")
add_subdirectory("VroomEditor")

# ----- Testing -----

include(CTest)