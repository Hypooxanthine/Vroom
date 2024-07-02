cmake_minimum_required(VERSION 3.8)

set(DART_TESTING_ENABLED OFF)

project(Vroom)

if (POLICY CMP0072)
    cmake_policy (SET CMP0072 NEW)
endif(POLICY CMP0072)
find_package(OpenGL REQUIRED)

# Project directories
set(SOURCE_DIR src)
set(INCLUDE_DIR include)

# Implementation
set(STB_IMAGE_IMPL ${DEPENDENCIES}/stb_image/src/stb_image.cpp)
set(PROJECT_IMPL
    ${SOURCE_DIR}/Core/Application.cpp
    ${SOURCE_DIR}/Core/Window.cpp

    ${SOURCE_DIR}/Event/GLFWEventsConverter.cpp
    ${SOURCE_DIR}/Event/Trigger/Trigger.cpp
    ${SOURCE_DIR}/Event/Trigger/TriggerManager.cpp
    ${SOURCE_DIR}/Event/Trigger/TriggerBinder.cpp
    ${SOURCE_DIR}/Event/CustomEvent/CustomEventManager.cpp
    ${SOURCE_DIR}/Event/CustomEvent/CustomEventBinder.cpp

    ${SOURCE_DIR}/Render/Renderer.cpp
    ${SOURCE_DIR}/Render/Abstraction/IndexBuffer.cpp
    ${SOURCE_DIR}/Render/Abstraction/Shader.cpp
    ${SOURCE_DIR}/Render/Abstraction/Texture.cpp
    ${SOURCE_DIR}/Render/Abstraction/VertexArray.cpp
    ${SOURCE_DIR}/Render/Abstraction/VertexBuffer.cpp
    ${SOURCE_DIR}/Render/Abstraction/VertexBufferLayout.cpp
    ${SOURCE_DIR}/Render/RenderObject/RenderMesh.cpp
    ${SOURCE_DIR}/Render/Camera/CameraBasic.cpp
    ${SOURCE_DIR}/Render/Camera/PerspectiveCameraBasic.cpp
    ${SOURCE_DIR}/Render/Camera/FirstPersonCamera.cpp

    ${SOURCE_DIR}/Scene/Scene.cpp
    ${SOURCE_DIR}/Scene/Entity.cpp
    ${SOURCE_DIR}/Scene/Components/MeshComponent.cpp

    ${SOURCE_DIR}/Asset/StaticAsset/StaticAsset.cpp
    ${SOURCE_DIR}/Asset/StaticAsset/MeshAsset.cpp
    ${SOURCE_DIR}/Asset/StaticAsset/TextureAsset.cpp
    ${SOURCE_DIR}/Asset/StaticAsset/MaterialAsset.cpp
    ${SOURCE_DIR}/Asset/AssetInstance/AssetInstance.cpp
    ${SOURCE_DIR}/Asset/AssetInstance/MeshInstance.cpp
    ${SOURCE_DIR}/Asset/AssetInstance/TextureInstance.cpp
    ${SOURCE_DIR}/Asset/AssetInstance/MaterialInstance.cpp
    ${SOURCE_DIR}/Asset/AssetData/MeshData.cpp
    ${SOURCE_DIR}/Asset/AssetData/MaterialData.cpp
    ${SOURCE_DIR}/Asset/Parsing/MaterialParsing.cpp
)

set(SOURCES
    ${STB_IMAGE_IMPL}
    ${PROJECT_IMPL}
)

set(INCLUDES
    ${INCLUDE_DIR}
    ${LIBS_INCLUDES}
)

include_directories(Vroom PUBLIC 
    ${INCLUDES}
)

add_library(Vroom STATIC ${SOURCES})

add_definitions(-D GLEW_STATIC)

target_link_libraries(Vroom
    ${OPENGL_LIBRARY}
    ${GLFW_LIBRARY}
    ${GLEW_LIBRARY}
)

add_subdirectory("tests")

include(CTest)