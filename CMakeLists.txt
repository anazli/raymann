cmake_minimum_required(VERSION 3.22)
project(raymann VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(RUN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(RUN_TEST_DIR ${PROJECT_SOURCE_DIR}/test/bin)
set(EXE raymann)

find_package(TBB REQUIRED)
find_package(Threads REQUIRED)

set(SOURCES src/acceleration/bounding_box.cpp
            src/acceleration/bounding_box.h
            src/acceleration/bvh.cpp
            src/acceleration/bvh.h
            src/application/error.h
            src/application/wavefront_reader.cpp
            src/application/wavefront_reader.h
            src/camera/camera.cpp
            src/camera/camera.h
            src/color/spectrum.cpp
            src/color/spectrum.h
            src/color/em_spectrum.cpp
            src/color/em_spectrum.h
            src/color/rgb_spectrum.cpp
            src/color/rgb_spectrum.h
            src/composite/builder.cpp
            src/composite/builder.h
            src/composite/iterator.cpp
            src/composite/iterator.h
            src/composite/scene_element.cpp
            src/composite/scene_element.h
            src/composite/world.cpp
            src/composite/world.h
            src/container/canvas.cpp
            src/container/canvas.h
            src/geometry/cone.h
            src/geometry/cube.h
            src/geometry/cylinder.h
            src/geometry/plane.h
            src/geometry/quad.h
            src/geometry/sphere.h
            src/geometry/triangle.h
            src/materials/material.cpp
            src/materials/material.h
            src/materials/material_properties.cpp
            src/materials/material_properties.h
            src/renderers/renderer.cpp
            src/renderers/renderer.h
            src/renderers/path_tracer.cpp
            src/renderers/path_tracer.h
            src/renderers/phong_model.cpp
            src/renderers/phong_model.h
            src/stochastic/pdf.cpp
            src/stochastic/pdf.h
            src/stochastic/random.cpp
            src/stochastic/random.h
            src/stochastic/stochastic_method.cpp
            src/stochastic/stochastic_method.h
            src/textures/perlin.cpp
            src/textures/perlin.h
            src/textures/texture.h
            src/transformations/scene_element_decorator.cpp
            src/transformations/scene_element_decorator.h
            src/transformations/transformer.cpp
            src/transformations/transformer.h
)

add_executable(${EXE} src/main/raymann.cpp ${SOURCES})
target_include_directories(${EXE} PRIVATE ${PROJECT_SOURCE_DIR}/src)
set_target_properties(${EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RUN_DIR})
target_compile_options(${EXE} PRIVATE "-g")
target_link_options(${EXE} PRIVATE "-lstdc++" PRIVATE "-lm")
target_link_libraries(${EXE} PRIVATE
                Threads::Threads
                TBB::tbb
)


if(BUILD_TESTING)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
  include(GoogleTest)


  set(TEST_EXE raymannTests)
  set(TEST_SOURCES test/gtesting.h
                   test/application_test.cpp
                   test/boundingBox_test.cpp
                   test/camera_test.cpp
                   test/canvas_test.cpp
                   test/color_test.cpp
                   test/material_test.cpp
                   test/primitive_test.cpp
                   test/properties_test.cpp
                   test/ray_test.cpp
                   test/tools_test.cpp
                   test/transformer_test.cpp
                   test/world_test.cpp
                   ${SOURCES}
  )

  add_executable(${TEST_EXE} ${TEST_SOURCES})
  target_include_directories(${TEST_EXE} PRIVATE ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/test)
  target_link_libraries(${TEST_EXE} gtest_main gtest pthread Threads::Threads TBB::tbb)
  set_target_properties(${TEST_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RUN_TEST_DIR})
  target_compile_options(${TEST_EXE} PRIVATE "-g")
  target_link_options(${TEST_EXE} PRIVATE "-lstdc++" PRIVATE "-lm")
  add_test(${TEST_EXE} ${RUN_TEST_DIR}/${TEST_EXE} WORKING_DIRECTORY ${RUN_TEST_DIR})
  gtest_discover_tests(${TEST_EXE})

  enable_testing()
  find_package(GTest REQUIRED)
  include_directories(${GTEST_INCLUDE_DIRS})

endif()
