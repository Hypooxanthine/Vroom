# ----- GLFW -----

set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
set(GLFW_LIBRARY_TYPE "SHARED")
add_subdirectory(glfw)

# ----- GLEW -----

add_subdirectory(glew)

# ----- IMGUI -----

# Manually setting the lib because imgui is not using CMake
set(imgui_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui/)

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
target_link_libraries(ImGuiGLFWGlew PUBLIC glfw libglew_shared)
target_compile_definitions(ImGuiGLFWGlew PUBLIC IMGUI_DEFINE_MATH_OPERATORS)

# ----- IMGUIZMO -----

set(imguizmo_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imguizmo/)

add_library(ImGuizmo
  ${imguizmo_SOURCE_DIR}/GraphEditor.cpp
  ${imguizmo_SOURCE_DIR}/ImCurveEdit.cpp
  ${imguizmo_SOURCE_DIR}/ImGradient.cpp
  ${imguizmo_SOURCE_DIR}/ImGuizmo.cpp
  ${imguizmo_SOURCE_DIR}/ImSequencer.cpp
)

target_include_directories(ImGuizmo PUBLIC ${imguizmo_SOURCE_DIR})
target_link_libraries(ImGuizmo PUBLIC ImGuiGLFWGlew)

# ----- IMPLOT -----

set(ImPlot_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/implot/)

add_library(ImPlot
  ${ImPlot_SOURCE_DIR}/implot.cpp
  ${ImPlot_SOURCE_DIR}/implot_items.cpp
)

target_include_directories(ImPlot PUBLIC ${ImPlot_SOURCE_DIR})
target_link_libraries(ImPlot PUBLIC ImGuiGLFWGlew)

# ----- GLM -----

add_subdirectory(glm)

# ----- SPDLOG -----

add_subdirectory(spdlog)

# ----- ENTT -----

add_subdirectory(entt)

# ----- Assimp -----

set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(assimp)

# ----- Jolt Physics -----

# Match Vroom's runtime library (dynamic /MD[d]). By default Jolt forces the
# static MSVC runtime (/MT[d]) on Windows, which causes RuntimeLibrary mismatch
# (LNK2038) and duplicate-symbol (LNK2005) errors when linking vrmPhysics.
set(USE_STATIC_MSVC_RUNTIME_LIBRARY OFF CACHE BOOL "" FORCE)
add_subdirectory(jolt/Build)
