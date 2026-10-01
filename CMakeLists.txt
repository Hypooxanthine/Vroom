cmake_minimum_required (VERSION 3.28)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API OFF)
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP OFF)

include(cmake/vroom.cmake)
include(cmake/modules.cmake)

# ----- CMake settings -----

# Policies
if (POLICY CMP0072)
    cmake_policy (SET CMP0072 NEW)
endif(POLICY CMP0072)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_EXPORT_COMPILE_COMMANDS AND EXISTS "${CMAKE_BINARY_DIR}/compile_commands.json")
  # Copy compile_commands.json to the source root so clangd finds it automatically.
  # On Windows, lowercase the drive letter (C: -> c:) so clangd's edit URIs match the
  # casing VS Code uses for opened documents. Without this, cross-file "Rename" fails
  # with "Rename failed to apply edits" because the drive-letter case differs. Everything
  # here lives on C:; on other platforms/drives the replace is simply a no-op.
  file(READ "${CMAKE_BINARY_DIR}/compile_commands.json" _vrm_ccjson)
  string(REPLACE "C:" "c:" _vrm_ccjson "${_vrm_ccjson}")
  file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json" "${_vrm_ccjson}")
  unset(_vrm_ccjson)
endif()


# ----- General compiler settings -----

# C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Visual Studio specific settings
if (CMAKE_GENERATOR MATCHES "Visual Studio")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT VroomEditor)
endif()

# ----- Project general definitions -----

# Root directory
get_filename_component(ROOT "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)

# Resource directories
set(ENGINE_RESOURCE_DIR   ${ROOT}/Vroom/Resources)

# ----- Solution definition -----

project ("VroomSolution")

# ----- Subdirectories -----

add_subdirectory("deps")
add_subdirectory("Vroom")

# ----- Testing -----

include(CTest)