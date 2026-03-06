# ----- Project definition -----

project(ScriptApi)

# ----- Project directories -----

set(SOURCE_DIR src)
set(INCLUDE_DIR include)

# ----- Projects files -----

# Script headers
file(GLOB_RECURSE HEADERS_FILES   ${INCLUDE_DIR}/*.h)

# Script sources
file(GLOB_RECURSE SOURCE_FILES    ${SOURCE_DIR}/*.cpp)

get_filename_component(SCRIPT_API_IMPL_FILE ${SOURCE_DIR}/ScriptApiImplementation.cpp ABSOLUTE)
list(REMOVE_ITEM SOURCE_FILES
  ${SCRIPT_API_IMPL_FILE}
)

# ----- Targets -----

add_library(ScriptApi                  STATIC  ${SOURCE_FILES} ${HEADERS_FILES})
target_include_directories(ScriptApi   PUBLIC  ${INCLUDE_DIR})
target_link_libraries(ScriptApi        PUBLIC  Vroom)
target_compile_definitions(ScriptApi   PRIVATE VRM_TARGET_SCRIPT_API)

# The implementation is meant to be compiled and linked by
# targets depending on ScriptApi only: user scripts plugin.
target_sources(
  ScriptApi
  INTERFACE ${SCRIPT_API_IMPL_FILE}
)

if(UNIX)
  target_compile_options(ScriptApi PRIVATE -fvisibility=hidden)
endif()

# Compile options
if (MSVC)
    target_compile_options(ScriptApi PRIVATE /MP)
endif()

# ----- Deps -----

