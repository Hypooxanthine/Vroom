cmake_minimum_required (VERSION 3.31)

set(DART_TESTING_ENABLED OFF)

# ----- Project definition -----

project(Vroom)

if (POLICY CMP0072)
    cmake_policy (SET CMP0072 NEW)
endif(POLICY CMP0072)

# ----- Dependency lib directories -----

include(FetchContent)

# glm
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
)
FetchContent_MakeAvailable(glm)

# spdlog
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.x
)
FetchContent_MakeAvailable(spdlog)

# entt
FetchContent_Declare(
    EnTT
    GIT_REPOSITORY https://github.com/skypjack/entt.git
)
FetchContent_MakeAvailable(EnTT)

FetchContent_Declare(
  json
  URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)


# Grouping the libraries
set(LIBRARIES 
    ${OPENGL_LIBRARY}
    glm::glm
    spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>
    EnTT::EnTT
    libglew_shared
    glfw
    nlohmann_json::nlohmann_json
)

# ----- Project directories -----

# Project directories
set(SOURCE_DIR  src)
set(INCLUDE_DIR include)
set(VENDOR_DIR  vendor)

# ----- Project files -----

# Header files
file(GLOB_RECURSE PROJECT_HEADERS   ${INCLUDE_DIR}/*.h)
file(GLOB_RECURSE VENDOR_HEADERS    ${VENDOR_DIR}/*.h)
set(HEADERS                         ${VENDOR_HEADERS} ${PROJECT_HEADERS})

# Source files
file(GLOB_RECURSE VENDOR_IMPL   ${VENDOR_DIR}/*.cpp)
file(GLOB_RECURSE PROJECT_IMPL  ${SOURCE_DIR}/*.cpp)
set(SOURCES                     ${VENDOR_IMPL} ${PROJECT_IMPL})

# ----- Binaries building -----
add_library(Vroom_static                   STATIC  ${SOURCES} ${HEADERS})
target_include_directories(Vroom_static    PUBLIC  ${VENDOR_DIR} ${INCLUDE_DIR})
target_link_libraries(Vroom_static                 ${LIBRARIES})
target_compile_definitions(Vroom_static PRIVATE "VRM_TARGET_VROOM" "VRM_STATIC")

add_library(Vroom                   SHARED  ${SOURCES} ${HEADERS})
target_include_directories(Vroom    PUBLIC  ${VENDOR_DIR} ${INCLUDE_DIR})
target_link_libraries(Vroom                 ${LIBRARIES})
target_compile_definitions(Vroom PRIVATE "VRM_TARGET_VROOM")
if(UNIX)
  target_compile_options(Vroom PRIVATE -fvisibility=hidden)
endif()

# ----- Specific settings -----

# Visual Studio
if (CMAKE_GENERATOR MATCHES "Visual Studio")
    # Grouping project files
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PROJECT_IMPL} ${HEADERS})
endif()

# Compile options
if (MSVC)
    target_compile_options(Vroom        PRIVATE /MP)
    target_compile_options(Vroom_static PRIVATE /MP)
endif()

# ----- Testing -----

add_subdirectory("tests")

include(CTest)