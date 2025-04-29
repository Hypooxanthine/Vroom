cmake_minimum_required (VERSION 3.31)

# ----- Project definition -----

project(Sandbox)

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

add_executable(Sandbox                      ${PROJECT_IMPL} ${PROJECT_HEADERS})
target_include_directories(Sandbox  PUBLIC  ${INCLUDE_DIR})
target_link_libraries(Sandbox               Vroom)

# Compile options
if (MSVC)
    target_compile_options(Sandbox PRIVATE /MP)
endif()

# ----- Specific settings -----

# Visual Studio specific settings
if (CMAKE_GENERATOR MATCHES "Visual Studio")
    # Grouping project files
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})
    # Set the debugger working directory
    set_property(TARGET Sandbox PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:Sandbox>)
endif()

# ----- Copying resource files -----

project(Resources NONE)

# Resource directories
set(ENGINE_RESOURCE_DIR ${CMAKE_SOURCE_DIR}/Resources)
set(SANDBOX_RESOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Resources)

# Output directory. For visual studio, we need to append the configuration name
if (CMAKE_GENERATOR MATCHES "Visual Studio")
    set(OUTPUT_DIR ${Sandbox_BINARY_DIR}/$<CONFIG>/Resources)
else()
    set(OUTPUT_DIR ${Sandbox_BINARY_DIR}/Resources)
endif()

add_custom_target(SandboxResources ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ENGINE_RESOURCE_DIR} ${OUTPUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SANDBOX_RESOURCE_DIR} ${OUTPUT_DIR}
    COMMENT "Copying resource files to output directory"
)

# Make sure Resources is built before Sandbox
add_dependencies(Sandbox SandboxResources)
