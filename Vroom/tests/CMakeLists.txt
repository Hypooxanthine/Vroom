cmake_minimum_required (VERSION 3.31)

project(VroomTests)

add_library(DummyLib SHARED test_RuntimeLibrary_DummyLib.cpp)
target_compile_definitions(DummyLib PRIVATE "DUM_TARGET_DUMMYLIB")
if(UNIX)
  target_compile_options(DummyLib PRIVATE -fvisibility=hidden)
endif()

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
target_compile_definitions(VroomTests PRIVATE "VRM_STATIC")

add_dependencies(VroomTests DummyLib)

enable_testing()

target_link_libraries(VroomTests
    Vroom_static
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(VroomTests)

include(CTest)

# Copy the resources to the build directory

set(OUTPUT_DIR $<TARGET_FILE_DIR:VroomTests>)
set(OUTPUT_RESOURCES_DIR ${OUTPUT_DIR}/Resources)

add_custom_command(TARGET VroomTests POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory
  ${OUTPUT_RESOURCES_DIR}
)

add_custom_command(TARGET VroomTests POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${ENGINE_RESOURCE_DIR}
  ${OUTPUT_RESOURCES_DIR}
)

add_custom_command(TARGET VroomTests POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  $<TARGET_FILE:DummyLib>
  ${OUTPUT_DIR}
)

get_target_property(VRM_TEST_DUMMYLIB_FILENAME DummyLib TARGET_FILE_NAME)
target_compile_definitions(VroomTests
  PUBLIC DUMMYLIB_FILENAME="$<TARGET_FILE_NAME:DummyLib>"
)

# Visual Studio specific settings
if (CMAKE_GENERATOR MATCHES "Visual Studio")
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "src" FILES ${TEST_SOURCES})
endif()