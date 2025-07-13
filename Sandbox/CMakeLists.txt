# ----- Project definition -----

project(Sandbox)

# ----- Project directories -----

set(SANDBOX_RESOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/Resources)
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
target_link_libraries(Sandbox               Vroom_static)
target_compile_definitions(Sandbox PRIVATE VRM_STATIC)

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

set(SANDBOX_RESOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Resources)
set(OUTPUT_DIR $<TARGET_FILE_DIR:Sandbox>/Resources)

add_custom_command(TARGET Sandbox POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory
  ${OUTPUT_DIR}
)

add_custom_command(TARGET Sandbox POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${ENGINE_RESOURCE_DIR} ${SANDBOX_RESOURCE_DIR}
  ${OUTPUT_DIR}
)
