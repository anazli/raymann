cmake_minimum_required(VERSION 3.16)
project(raymann VERSION   3.14159 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(RUN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(RUN_TEST_DIR ${PROJECT_SOURCE_DIR}/test/bin)
set(EXE raymann)

set(sources src/main/raymann.cpp
            src/geometry/traceable.cpp
            src/container/canvas.cpp
            src/world/world.cpp
            src/world/scene.cpp)

add_executable(${EXE} ${sources})
target_include_directories(${EXE} PUBLIC ${PROJECT_SOURCE_DIR}/src)
set_target_properties(${EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RUN_DIR})


if(BUILD_TESTING)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/2d07f12b607c528b21795ab672cff3afaf64f7a1.zip
  )
  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
  include(GoogleTest)


  MACRO(TESTCASE Name)
    set(test Test${Name})
    add_executable(${test} test/test${Name}.cpp src/geometry/traceable.cpp
                                                 src/container/canvas.cpp
                                                 src/world/world.cpp)

    target_include_directories(${test} PUBLIC ${PROJECT_SOURCE_DIR}/src)
    target_link_libraries(${test} gtest_main gtest pthread)
    set_target_properties(${test} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RUN_TEST_DIR})
    add_test(${test} ${RUN_TEST_DIR}/${test})
    gtest_discover_tests(${test})
  ENDMACRO(TESTCASE)

  enable_testing()
  find_package(GTest REQUIRED)
  include_directories(${GTEST_INCLUDE_DIRS})

  TESTCASE(Tools)
  TESTCASE(Canvas)
  TESTCASE(Trans)
  TESTCASE(Ray)
  TESTCASE(Sphere)
  TESTCASE(World)
  TESTCASE(Mat)
endif()
