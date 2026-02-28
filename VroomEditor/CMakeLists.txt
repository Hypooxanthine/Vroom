
# ----- Options -----

option(VROOM_ENABLE_EXAMPLE_EDITOR "Enable Vroom editor module" ON)

# ----- Subdirectories -----

if (VROOM_ENABLE_EXAMPLE_EDITOR)
  add_subdirectory("Scripts")
endif()

# ----- Project definition -----

project(VroomEditor)

# ----- Dependency lib directories -----

# Grouping the libraries

set(LIBRARIES 
  ImGuiGLFWGlew
  ImGuizmo
  ImPlot
  Vroom
  ScriptApi
)

# ----- Project directories -----

set(SOURCE_DIR src)
set(INCLUDE_DIR include)

# ----- Project files -----

# Header files
file(GLOB_RECURSE PROJECT_HEADERS   ${INCLUDE_DIR}/*.h)
set(HEADERS ${PROJECT_HEADERS})

# Source files
file(GLOB_RECURSE PROJECT_IMPL      ${SOURCE_DIR}/*.cpp)
set(SOURCES ${PROJECT_IMPL})

# ----- Binaries building -----

add_executable(VroomEditor                      ${PROJECT_IMPL} ${PROJECT_HEADERS})
target_include_directories(VroomEditor  PUBLIC  ${INCLUDE_DIR} ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
target_link_libraries(VroomEditor       PUBLIC  ${LIBRARIES})

# Compile options
if (MSVC)
    target_compile_options(VroomEditor PRIVATE /MP)
endif()

# ----- Specific settings -----

# Visual Studio specific settings
if (CMAKE_GENERATOR MATCHES "Visual Studio")
    # Grouping project files
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})
    # Set the debugger working directory
    set_property(TARGET VroomEditor PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:VroomEditor>)
endif()

# ----- Copying resource files -----

if (WIN32)
  set(SCRIPTS_BUILDER ${CMAKE_CURRENT_SOURCE_DIR}/scriptsBuilder.bat)
elseif (UNIX)
  set(SCRIPTS_BUILDER ${CMAKE_CURRENT_SOURCE_DIR}/scriptsBuilder.sh)
endif()

add_resources_target(VroomEngineResources ${ENGINE_RESOURCE_DIR})
add_resources_target(VroomEditorResources ${CMAKE_CURRENT_SOURCE_DIR}/Resources)

if (VROOM_ENABLE_EXAMPLE_EDITOR)
  add_resources_target(VroomEditorDefaultResources ${CMAKE_CURRENT_SOURCE_DIR}/DefaultResources/Resources)
endif()

add_custom_target(VroomEditorOtherResources
  COMMAND ${CMAKE_COMMAND} -E copy
    # Shared/runtime libraries
    $<TARGET_FILE:Vroom> $<TARGET_FILE:glfw> $<TARGET_FILE:libglew_shared>
    # Imgui default config
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui.ini
    $<TARGET_FILE_DIR:VroomEditor>
)

add_dependencies(VroomEditor VroomEditorOtherResources)
add_dependencies(VroomEditorOtherResources Vroom glfw libglew_shared)

if (VROOM_ENABLE_EXAMPLE_EDITOR)
  set_scripts_library(ScriptsLibrary)
endif()

target_compile_definitions(VroomEditor
  PRIVATE VRM_SCRIPT_BUILDER_CMD=\"${SCRIPTS_BUILDER}\"
  PRIVATE VRM_CMAKE_CMD=\"${CMAKE_COMMAND}\"
  PRIVATE VRM_BUILD_DIR=\"${CMAKE_BINARY_DIR}\"
)
