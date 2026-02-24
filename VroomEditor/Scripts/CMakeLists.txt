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
target_include_directories(ScriptsLibrary   PRIVATE  ${INCLUDE_DIR})
target_link_libraries(ScriptsLibrary        PRIVATE  Vroom ScriptApi)

# Those symbols are defined in ScriptApi and must be exported by the shared library to be accessible at runtime
# But since ScriptLibrary does not use them directly, the linker may not include them in the ScriptLibrary DLL. We need to force their inclusion.
if (MSVC)
  target_link_options(ScriptsLibrary PRIVATE
    "/INCLUDE:VRM_ScriptLibrary_GetLibraryName"
    "/INCLUDE:VRM_ScriptLibrary_GetScriptCount"
    "/INCLUDE:VRM_ScriptLibrary_GetScriptName"
    "/INCLUDE:VRM_ScriptLibrary_CreateScript"
    "/INCLUDE:VRM_ScriptLibrary_DestroyScript"
  )
else ()
  target_link_options(ScriptsLibrary PRIVATE
    "-Wl,--export=VRM_ScriptLibrary_GetLibraryName"
    "-Wl,--export=VRM_ScriptLibrary_GetScriptCount"
    "-Wl,--export=VRM_ScriptLibrary_GetScriptName"
    "-Wl,--export=VRM_ScriptLibrary_CreateScript"
    "-Wl,--export=VRM_ScriptLibrary_DestroyScript"
  )
endif()

# Compile options
if (MSVC)
    target_compile_options(ScriptsLibrary PRIVATE /MP)
endif()

# ----- Deps -----

