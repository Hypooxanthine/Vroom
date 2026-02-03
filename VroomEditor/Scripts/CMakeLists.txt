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
target_compile_definitions(ScriptsLibrary   PRIVATE "VRM_TARGET_SCRIPT_LIBRARY")

# Compile options
if (MSVC)
    target_compile_options(ScriptsLibrary PRIVATE /MP)
endif()

# ----- Deps -----

