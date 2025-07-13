add_subdirectory("Scripts")

# ----- Project definition -----

project(VroomEditor)

# ----- Dependency lib directories -----

# Grouping the libraries

set(LIBRARIES 
  ImGuiGLFWGlew
  ImGuizmo
  Vroom
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

set(OUTPUT_DIR $<TARGET_FILE_DIR:VroomEditor>)
set(OUTPUT_RESOURCES_DIR ${OUTPUT_DIR}/Resources)

if (WIN32)
  set(SCRIPTS_BUILDER ${CMAKE_CURRENT_SOURCE_DIR}/scriptsBuilder.bat)
elseif (UNIX)
  set(SCRIPTS_BUILDER ${CMAKE_CURRENT_SOURCE_DIR}/scriptsBuilder.sh)
endif()

add_custom_target(VroomEditorResources
  COMMAND ${CMAKE_COMMAND} -E make_directory
    # Create /Resources dir
    ${OUTPUT_RESOURCES_DIR}

  COMMAND ${CMAKE_COMMAND} -E copy
    # Copy shared/runtime libraries
    $<TARGET_FILE:Vroom> $<TARGET_FILE:glfw> $<TARGET_FILE:libglew_shared> $<TARGET_FILE:ScriptsLibrary> ${SCRIPTS_BUILDER}
    # Imgui default config
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui.ini
    ${OUTPUT_DIR}
  
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    # Copy Resources dirs
    ${ENGINE_RESOURCE_DIR} ${EDITOR_RESOURCE_DIR}
    ${OUTPUT_RESOURCES_DIR}
)

add_dependencies(VroomEditor VroomEditorResources)
add_dependencies(VroomEditorResources Vroom glfw libglew_shared ScriptsLibrary)

target_compile_definitions(VroomEditor
  PRIVATE VRM_RUNTIME_SCRIPTS_PATH="$<TARGET_FILE_NAME:ScriptsLibrary>"
)

target_compile_definitions(VroomEditor
  PRIVATE VRM_SCRIPT_BUILDER_CMD="./${SCRIPTS_BUILDER} ${CMAKE_COMMAND} ${CMAKE_BINARY_DIR}"
)

