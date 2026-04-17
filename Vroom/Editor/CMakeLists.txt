# ----- Module definition -----

project(Editor)
add_module(Editor)

# ----- Dependencies -----

module_link_modules(Editor Application)
module_link_library(
  Editor
  PUBLIC
    ImGuizmo
    ImPlot
    assimp
)

# ----- Compile definitions -----

module_compile_definition(
  Editor
  PRIVATE VRM_EDITOR_EXPORTS=1
)

# ----- Compile options -----

# ----- Executable target -----

option(VROOM_ENABLE_EXAMPLE_EDITOR "Enable Vroom editor module" ON)

if (VROOM_ENABLE_EXAMPLE_EDITOR)
  add_subdirectory("DefaultScripts")
endif()

add_executable(VroomEditor                "main.cpp")
target_link_libraries(VroomEditor PRIVATE Editor_protected)
set_target_properties(VroomEditor PROPERTIES
  VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:VroomEditor>"
)

if (WIN32)
  set(SCRIPTS_BUILDER ${CMAKE_CURRENT_SOURCE_DIR}/scriptsBuilder.bat)
elseif (UNIX)
  set(SCRIPTS_BUILDER ${CMAKE_CURRENT_SOURCE_DIR}/scriptsBuilder.sh)
endif()

add_resources_target(VroomEngineResources ${ENGINE_RESOURCE_DIR})
add_resources_target(VroomEditorResources ${CMAKE_CURRENT_SOURCE_DIR}/Resources)

if (VROOM_ENABLE_EXAMPLE_EDITOR)
  set_game_rsc_dir(${CMAKE_CURRENT_SOURCE_DIR}/DefaultResources)
  set_scripts_library(DefaultScripts)
endif()

set(VROOM_EDITOR_RUNTIME_COPY_COMMAND)
if(WIN32)
  list(APPEND VROOM_EDITOR_RUNTIME_COPY_COMMAND
    COMMAND ${CMAKE_COMMAND} -E copy
      # Shared/runtime libraries
      $<TARGET_RUNTIME_DLLS:VroomEditor>
      $<TARGET_FILE_DIR:VroomEditor>
  )
endif()

add_custom_target(VroomEditorOtherResources
  COMMAND ${CMAKE_COMMAND} -E make_directory
    # Create dir
    $<TARGET_FILE_DIR:VroomEditor>

  COMMAND ${CMAKE_COMMAND} -E copy
    # Imgui default config
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui.ini
    $<TARGET_FILE_DIR:VroomEditor>

  ${VROOM_EDITOR_RUNTIME_COPY_COMMAND}

  COMMAND_EXPAND_LISTS
)

add_dependencies(VroomEditor VroomEditorOtherResources)
add_dependencies(VroomEditorOtherResources Editor_private)

target_compile_definitions(VroomEditor
  PRIVATE VRM_SCRIPT_BUILDER_CMD=\"${SCRIPTS_BUILDER}\"
  PRIVATE VRM_CMAKE_CMD=\"${CMAKE_COMMAND}\"
  PRIVATE VRM_BUILD_DIR=\"${CMAKE_BINARY_DIR}\"
)
