cmake_minimum_required(VERSION 3.8)

project(Sandbox)

set(SOURCE_DIR src)
set(INCLUDE_DIR include)

set(SOURCES
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/Scripts/SuzanneScript.cpp
    ${SOURCE_DIR}/Scripts/MovingLightScript.cpp
)

set(INCLUDES
    ${INCLUDE_DIR}
    ${VROOM}/include
    ${LIBS_INCLUDES}
)

include_directories(Sandbox PUBLIC 
    ${INCLUDES}
)

add_executable(Sandbox ${SOURCES})

target_link_libraries(Sandbox
    Vroom
)

set_property(TARGET Sandbox PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:Sandbox>)

# Copy the resources to the build directory (engine)
add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Resources
    $<TARGET_FILE_DIR:Sandbox>/Resources
)

# Copy the resources to the build directory (sandbox)
add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/Resources
    $<TARGET_FILE_DIR:Sandbox>/Resources
)