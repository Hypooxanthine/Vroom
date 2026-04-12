# ----- Project definition -----

project(ScriptApi)

# ----- Project directories -----

set(SOURCE_DIR src)

# ----- Projects files -----

# Script sources
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_DIR}/*.cpp)

get_filename_component(SCRIPT_API_IMPL_FILE ${SOURCE_DIR}/ScriptApiImplementation.cpp ABSOLUTE)
list(REMOVE_ITEM SOURCE_FILES
  ${SCRIPT_API_IMPL_FILE}
)

# ----- Targets -----

add_library(
  ScriptApi
  STATIC ${SOURCE_FILES}
)

target_include_directories(ScriptApi
  PUBLIC "public"
  PRIVATE "private" "protected"
)

target_link_libraries(
  ScriptApi
  PRIVATE
    Application_protected
  PUBLIC
    Application_public
)

add_library(Vroom::ScriptApi ALIAS ScriptApi)

# The implementation is meant to be compiled and linked by
# targets depending on ScriptApi only: user scripts plugin.
target_sources(
  ScriptApi
  INTERFACE
    ${SCRIPT_API_IMPL_FILE}
)

if(UNIX)
  target_compile_options(ScriptApi PRIVATE -fvisibility=hidden)
endif()

# Compile options
if (MSVC)
    target_compile_options(ScriptApi PRIVATE /MP)
endif()

set_scripts_library(DefaultScripts)

# ----- Deps -----

