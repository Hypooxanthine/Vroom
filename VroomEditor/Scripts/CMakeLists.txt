cmake_minimum_required (VERSION 3.31)

# ----- Project definition -----

project(ScriptsLibrary)

# ----- Project directories -----

set(SOURCE_DIR src)
set(INCLUDE_DIR include)

# ----- Projects files -----

# Script headers
file(GLOB_RECURSE SCRIPT_HEADERS   ${INCLUDE_DIR}/*.h)

# Script sources
file(GLOB_RECURSE SCRIPT_IMPL      ${SOURCE_DIR}/*.cpp)

# ----- Targets -----

add_library(ScriptsLibrary                  SHARED  ${SCRIPT_IMPL} ${SCRIPT_HEADERS})
target_include_directories(ScriptsLibrary   PUBLIC  ${INCLUDE_DIR})
target_link_libraries(ScriptsLibrary        PUBLIC  Vroom)
target_compile_definitions(ScriptsLibrary   PRIVATE SCRIPTSLIBRARY_EXPORTS)

add_dependencies(ScriptsLibrary VroomEditor)

add_custom_command(TARGET ScriptsLibrary POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  $<TARGET_FILE:ScriptsLibrary>
  $<TARGET_FILE_DIR:VroomEditor>
)

add_library(ScriptsLibrary_static                  STATIC  ${SCRIPT_IMPL} ${SCRIPT_HEADERS})
target_include_directories(ScriptsLibrary_static   PUBLIC  ${INCLUDE_DIR})
target_link_libraries(ScriptsLibrary_static        PUBLIC  Vroom)

# Compile options
if (MSVC)
    target_compile_options(ScriptsLibrary PRIVATE /MP)
endif()

# ----- Deps -----

