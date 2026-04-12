# ----- Project definition -----

project(DefaultScripts)

# ----- Project directories -----

set(SOURCE_DIR src)
set(INCLUDE_DIR include)

# ----- Projects files -----

# Script headers
file(GLOB_RECURSE SCRIPT_HEADERS   ${INCLUDE_DIR}/*.h)

# Script sources
file(GLOB_RECURSE SCRIPT_IMPL      ${SOURCE_DIR}/*.cpp)

# ----- Targets -----

add_library(DefaultScripts                  SHARED  ${SCRIPT_IMPL} ${SCRIPT_HEADERS})
target_include_directories(DefaultScripts   PRIVATE  ${INCLUDE_DIR})
target_link_libraries(DefaultScripts        PRIVATE  Vroom::ScriptApi)

# Compile options

if(UNIX)
  target_compile_options(DefaultScripts PRIVATE -fvisibility=hidden)
endif()

if (MSVC)
    target_compile_options(DefaultScripts PRIVATE /MP)
endif()

# ----- Deps -----
