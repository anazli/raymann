cmake_minimum_required(VERSION 3.16)
project(raymann VERSION   3.14159 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(RUN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(RUN_TEST_DIR ${PROJECT_SOURCE_DIR}/test/bin)
set(EXE raymann)

set(SOURCES src/main/raymann.cpp
            src/builder/shape_builder.cpp
            src/composite/traceable.cpp
            src/composite/scene_director.cpp
            src/composite/properties.cpp
            src/container/canvas.cpp
            src/composite/world.cpp
            src/decorators/material.cpp
            src/decorators/traceable_deco.cpp
            src/decorators/transformer.cpp)

add_executable(${EXE} ${SOURCES})
target_include_directories(${EXE} PRIVATE ${PROJECT_SOURCE_DIR}/src)
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


  MACRO(TESTCASE NAME)
    set(TEST_EXE ${NAME}_test)
    add_executable(${TEST_EXE} test/${NAME}_test.cpp
        src/composite/traceable.cpp
        src/builder/shape_builder.cpp
        src/container/canvas.cpp
        src/composite/world.cpp
        src/composite/scene_director.cpp
        src/composite/properties.cpp
        src/decorators/transformer.cpp
        src/decorators/material.cpp
        src/decorators/traceable_deco.cpp)

    target_include_directories(${TEST_EXE} PRIVATE ${PROJECT_SOURCE_DIR}/src)
    target_link_libraries(${TEST_EXE} gtest_main gtest pthread)
    set_target_properties(${TEST_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RUN_TEST_DIR})
    add_test(${TEST_EXE} ${RUN_TEST_DIR}/${TEST_EXE})
    gtest_discover_tests(${TEST_EXE})
  ENDMACRO(TESTCASE)

  enable_testing()
  find_package(GTest REQUIRED)
  include_directories(${GTEST_INCLUDE_DIRS})

  TESTCASE(tools)
  TESTCASE(canvas)
  TESTCASE(transformer)
  TESTCASE(ray)
  TESTCASE(sphere)
  TESTCASE(world)
  TESTCASE(material)
  TESTCASE(camera)
  TESTCASE(plane)
endif()
