﻿cmake_minimum_required(VERSION 3.8)

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