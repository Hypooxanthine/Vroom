cmake_minimum_required(VERSION 3.8)

project(VroomTests)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(Vroom PUBLIC 
    ${INCLUDES}
)

set(TEST_SOURCES
    "test_TriggerCallbackList.cc"
    "test_Trigger.cc"
    "test_TriggerManagerAndBinder.cc"
    "test_CustomEvent.cc"
    "test_CustomEventManagerAndBinder.cc"
)

add_executable(VroomTests ${TEST_SOURCES})

target_compile_definitions(VroomTests PUBLIC -D GLEW_STATIC)

enable_testing()

target_link_libraries(VroomTests
    Vroom
    ${OPENGL_LIBRARY}
    ${GLFW_LIBRARY}
    ${GLEW_LIBRARY}
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(VroomTests)

include(CTest)