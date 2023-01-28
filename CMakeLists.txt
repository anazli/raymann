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
            src/decorators/transformer.cpp
            src/textures/perlin.cpp)

add_executable(${EXE} ${SOURCES})
target_include_directories(${EXE} PRIVATE ${PROJECT_SOURCE_DIR}/src)
set_target_properties(${EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RUN_DIR})


if(BUILD_TESTING)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
  include(GoogleTest)


  MACRO(TESTCASE NAME SOURCES)
    set(TEST_EXE ${NAME}_test)
    add_executable(${TEST_EXE} test/${NAME}_test.cpp ${SOURCES})
    target_include_directories(${TEST_EXE} PRIVATE ${PROJECT_SOURCE_DIR}/src)
    target_link_libraries(${TEST_EXE} gtest_main gtest pthread)
    set_target_properties(${TEST_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RUN_TEST_DIR})
    add_test(${TEST_EXE} ${RUN_TEST_DIR}/${TEST_EXE})
    gtest_discover_tests(${TEST_EXE})
  ENDMACRO(TESTCASE)

  enable_testing()
  find_package(GTest REQUIRED)
  include_directories(${GTEST_INCLUDE_DIRS})

  set(testSources src/tools/tools.h)
  TESTCASE(tools ${testSources})
  set(testSources src/container/canvas.cpp)
  TESTCASE(canvas ${testSources})
  set(testSources src/decorators/transformer.cpp)
  TESTCASE(transformer ${testSources})
  set(testSources src/decorators/transformer.cpp)
  TESTCASE(ray ${testSources})
  set(testSources src/geometry/sphere.h)
  TESTCASE(sphere ${testSources})
  set(testSources
      src/composite/traceable.cpp
      src/builder/shape_builder.cpp
      src/container/canvas.cpp
      src/composite/world.cpp
      src/composite/scene_director.cpp
      src/composite/properties.cpp
      src/decorators/transformer.cpp
      src/decorators/material.cpp
      src/decorators/traceable_deco.cpp
      src/textures/perlin.cpp)
  TESTCASE(world ${testSources})
  set(testSources src/decorators/material.cpp)
  TESTCASE(material ${testSources})
  set(testSources src/camera/camera.h)
  TESTCASE(camera ${testSources})
  set(testSources src/geometry/plane.h)
  TESTCASE(plane ${testSources})
  set(testSources src/composite/properties.cpp)
  TESTCASE(properties ${testSources})
endif()
