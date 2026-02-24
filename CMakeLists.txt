cmake_minimum_required (VERSION 3.28)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API OFF)
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP OFF)

# ----- CMake settings -----

# Policies
if (POLICY CMP0072)
    cmake_policy (SET CMP0072 NEW)
endif(POLICY CMP0072)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_EXPORT_COMPILE_COMMANDS AND EXISTS "${CMAKE_BINARY_DIR}/compile_commands.json")
  file(COPY "${CMAKE_BINARY_DIR}/compile_commands.json" DESTINATION "${CMAKE_SOURCE_DIR}")
endif()


# ----- General compiler settings -----

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Warnings
# if (UNIX)
#     add_compile_options(-Wno-volatile)
# endif(UNIX)

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
get_filename_component(ROOT "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)

# Resource directories
set(ENGINE_RESOURCE_DIR   ${ROOT}/Vroom/Resources)
set(EDITOR_RESOURCE_DIR   ${ROOT}/VroomEditor/Resources)

# ----- Solution definition -----

project ("VroomSolution")

# ----- Subdmodules -----

add_subdirectory("deps")

# ----- Subdirectories -----

add_subdirectory ("Vroom")
add_subdirectory("ScriptApi")
add_subdirectory("Sandbox")
add_subdirectory("VroomEditor")

# ----- Testing -----

include(CTest)