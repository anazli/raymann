cmake_minimum_required(VERSION 3.22)
project(raymann VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(RUN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(RUN_TEST_DIR ${PROJECT_SOURCE_DIR}/test/bin)
set(EXE raymann)

set(SOURCES src/main/raymann.cpp
            src/composite/builder.cpp
            src/composite/scene_element.cpp
            src/container/canvas.cpp
            src/composite/world.cpp
            src/geometry/bounding_box.h
            src/geometry/sphere.h
            src/geometry/plane.h
            src/materials/material.cpp
            src/materials/material_properties.h
            src/materials/material_properties.cpp
            src/transformations/scene_element_decorator.cpp
            src/transformations/transformer.cpp
            src/renderers/phong_model.cpp
            src/renderers/path_tracer.cpp
            src/textures/perlin.cpp)

add_executable(${EXE} ${SOURCES})
target_include_directories(${EXE} PRIVATE ${PROJECT_SOURCE_DIR}/src)
set_target_properties(${EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RUN_DIR})
#target_compile_options(${EXE} PRIVATE "-g")
target_link_options(${EXE} PRIVATE "-lstdc++" PRIVATE "-lm")


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


  MACRO(TESTCASE NAME TEST_SOURCES)
    set(TEST_EXE ${NAME}_test)
    add_executable(${TEST_EXE} test/${NAME}_test.cpp ${TEST_SOURCES})
    target_include_directories(${TEST_EXE} PRIVATE ${PROJECT_SOURCE_DIR}/src)
    target_link_libraries(${TEST_EXE} gtest_main gtest pthread)
    set_target_properties(${TEST_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RUN_TEST_DIR})
    #target_compile_options(${TEST_EXE} PRIVATE "-g")
    target_link_options(${TEST_EXE} PRIVATE "-lstdc++" PRIVATE "-lm")
    add_test(${TEST_EXE} ${RUN_TEST_DIR}/${TEST_EXE})
    gtest_discover_tests(${TEST_EXE})
  ENDMACRO(TESTCASE)

  enable_testing()
  find_package(GTest REQUIRED)
  include_directories(${GTEST_INCLUDE_DIRS})

  set(testSources src/tools/tools.h)
  TESTCASE(tools ${testSources})
  set(testSources src/container/canvas.cpp
      src/composite/scene_element.cpp
     )
  TESTCASE(canvas ${testSources})
  set(testSources src/tools/ray.h)
  TESTCASE(ray ${testSources})
  set(testSources src/camera/camera.h)
  TESTCASE(camera ${testSources})
  set(testSources src/materials/material_properties.cpp)
  TESTCASE(properties ${testSources})
  set(testSources src/composite/scene_element.cpp
      src/geometry/bounding_box.h
      src/geometry/sphere.h
      src/geometry/plane.h
      src/transformations/transformer.cpp
      src/transformations/scene_element_decorator.cpp
      src/textures/perlin.cpp)
  TESTCASE(sphere "${testSources}")
  TESTCASE(plane "${testSources}")
  TESTCASE(boundingBox "${testSources}")
  set(testSources
      src/composite/scene_element.cpp
      src/composite/builder.cpp
      src/composite/world.cpp
      src/materials/material.cpp
      src/materials/material_properties.cpp
      src/transformations/scene_element_decorator.cpp
      src/transformations/transformer.cpp
      src/textures/perlin.cpp
      src/renderers/phong_model.cpp)
  TESTCASE(material "${testSources}")
  TESTCASE(transformer "${testSources}")
  TESTCASE(world "${testSources}")
endif()
