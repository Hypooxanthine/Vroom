cmake_minimum_required(VERSION 3.8)

project(Sandbox)

set(SOURCE_DIR src)
set(INCLUDE_DIR include)

set(SOURCES
    ${SOURCE_DIR}/main.cpp
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

add_custom_target(
    TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    ${RESOURCES} $<TARGET_FILE_DIR:Sandbox>/Resources/
)