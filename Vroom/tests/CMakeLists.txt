cmake_minimum_required (VERSION 3.31)

project(VroomTests)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG 52eb810
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(Vroom PUBLIC 
    ${INCLUDES}
)

file(GLOB_RECURSE TEST_SOURCES  *.cc)

add_executable(VroomTests ${TEST_SOURCES})

target_compile_definitions(VroomTests PUBLIC -D GLEW_STATIC)

enable_testing()

target_link_libraries(VroomTests
    Vroom
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(VroomTests)

include(CTest)

# Copy the resources to the build directory
add_custom_command(TARGET VroomTests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Resources
    $<TARGET_FILE_DIR:VroomTests>/Resources
)

# Visual Studio specific settings
if (CMAKE_GENERATOR MATCHES "Visual Studio")
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "src" FILES ${TEST_SOURCES})
endif()