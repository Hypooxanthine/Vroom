cmake_minimum_required (VERSION 3.31)

# ----- Project definition -----

project(VroomEditor)

# ----- Dependency lib directories -----

include(FetchContent)

# imgui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY  https://github.com/ocornut/imgui.git
    GIT_TAG         docking
)
FetchContent_MakeAvailable(imgui)

add_library(ImGuiGLFWGlew
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(ImGuiGLFWGlew PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
target_link_libraries(ImGuiGLFWGlew PUBLIC glfw libglew_static)

# Grouping the libraries

set(LIBRARIES 
    ImGuiGLFWGlew
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
target_link_libraries(VroomEditor       PUBLIC  ${LIBRARIES} Vroom)

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

project(Resources NONE)

# Resource directories
set(ENGINE_RESOURCE_DIR         ${CMAKE_SOURCE_DIR}/Resources)
set(VROOMEDITOR_RESOURCE_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/Resources)

# Output directory. For visual studio, we need to append the configuration name
if (CMAKE_GENERATOR MATCHES "Visual Studio")
    set(OUTPUT_DIR ${VroomEditor_BINARY_DIR}/$<CONFIG>/Resources)
else()
    set(OUTPUT_DIR ${VroomEditor_BINARY_DIR}/Resources)
endif()

add_custom_target(VroomEditorResources ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ENGINE_RESOURCE_DIR} ${OUTPUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${VROOMEDITOR_RESOURCE_DIR} ${OUTPUT_DIR}
    COMMENT "Copying resource files to output directory"
)

# Make sure Resources is built before VroomEditor
add_dependencies(VroomEditor VroomEditorResources)
