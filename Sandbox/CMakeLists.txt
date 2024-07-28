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

project(Resources NONE)

set(ENGINE_RESOURCE_DIR ${CMAKE_SOURCE_DIR}/Resources)
set(SANDBOX_RESOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Resources)
set(OUTPUT_DIR $<TARGET_FILE_DIR:Sandbox>/Resources)

file(GLOB_RECURSE RESOURCE_FILES
     "${ENGINE_RESOURCE_DIR}/*.*"
     "${SANDBOX_RESOURCE_DIR}/*.*"
)

add_custom_target(Resources ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ENGINE_RESOURCE_DIR} ${OUTPUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SANDBOX_RESOURCE_DIR} ${OUTPUT_DIR}
    DEPENDS ${RESOURCE_FILES}
    COMMENT "Copying resource files to output directory"
)

foreach(RESOURCE ${RESOURCE_FILES})
    add_custom_command(TARGET Resources POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${RESOURCE} ${OUTPUT_DIR}
    )
endforeach()
