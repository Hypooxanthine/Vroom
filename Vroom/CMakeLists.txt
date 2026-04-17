set(DART_TESTING_ENABLED OFF)

# ----- Project definition -----

project(Vroom)

if (POLICY CMP0072)
  cmake_policy (SET CMP0072 NEW)
endif(POLICY CMP0072)

# ----- Specific settings -----

# Compile options
if (MSVC)
  add_compile_options(/MP)
endif()

# ----- Testing -----

add_subdirectory("vendor")
add_subdirectory("Core")
add_subdirectory("Tools")
add_subdirectory("Event")
add_subdirectory("Window")
add_subdirectory("ScriptEngine")
add_subdirectory("Rasterizer")
add_subdirectory("RenderObjects")
add_subdirectory("AssetManager")
add_subdirectory("Renderer")
add_subdirectory("Scene")
add_subdirectory("Application")
add_subdirectory("tests")
add_subdirectory("Editor")
add_subdirectory("ScriptApi")
# VRM-SUBDIRS-MARKERS
# Don't touch the line above, it is used as a marker
# for the module-creator script

include(CTest)
