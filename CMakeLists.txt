cmake_minimum_required(VERSION 3.25)
project(raymann VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(raymann CXX)
set(RUN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(RUN_TEST_DIR ${PROJECT_SOURCE_DIR}/test/bin)
set(LIBRARY_NAME l${PROJECT_NAME})

add_library(${LIBRARY_NAME} 
            src/acceleration/bounding_box.cpp
            src/acceleration/bounding_box.h
            src/acceleration/bvh.cpp
            src/acceleration/bvh.h
            src/application/error.h
            src/application/parameters.h
            src/application/data_container.cpp
            src/application/data_container.h
            src/application/wavefront_reader.cpp
            src/application/wavefront_reader.h
            src/camera/camera.cpp
            src/camera/camera.h
            src/color/spectrum.cpp
            src/color/spectrum.h
            src/composite/builder.cpp
            src/composite/builder.h
            src/composite/intersection.h
            src/composite/iterator.cpp
            src/composite/iterator.h
            src/composite/scene_element.cpp
            src/composite/scene_element.h
            src/composite/world.cpp
            src/composite/world.h
            src/container/canvas.cpp
            src/container/canvas.h
            src/distribution/bxdf.cpp
            src/distribution/bxdf.h
            src/distribution/lambertian_brdf.cpp
            src/distribution/lambertian_brdf.h
            src/geometry/cone.h
            src/geometry/cube.h
            src/geometry/cylinder.h
            src/geometry/plane.h
            src/geometry/quad.h
            src/geometry/sphere.h
            src/geometry/triangle.h
            src/materials/material.cpp
            src/materials/material.h
            src/materials/dielectric.cpp
            src/materials/dielectric.h
            src/materials/emissive.cpp
            src/materials/emissive.h
            src/materials/isotropic.cpp
            src/materials/isotropic.h
            src/materials/lambertian.cpp
            src/materials/lambertian.h
            src/materials/metal.cpp
            src/materials/metal.h
            src/materials/standard.cpp
            src/materials/standard.h
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
            src/textures/texture.cpp
            src/textures/texture.h
            src/transformations/transformation.cpp
            src/transformations/transformation.h
            src/tools/mat2.h
            src/tools/mat3.h
            src/tools/mat4.h
            src/tools/normal3.h
            src/tools/light.h
            src/tools/orthonormal.h
            src/tools/point3.h
            src/tools/ray.h
            src/tools/vec2.h
            src/tools/vec3.h
            src/tools/vec4.h
            src/tools/tools.h
)

target_include_directories(${LIBRARY_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME} src/main/raymann.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RUN_DIR})
target_compile_options(${PROJECT_NAME} PRIVATE "-g")
target_link_options(${PROJECT_NAME} PRIVATE "-lstdc++" PRIVATE "-lm")
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBRARY_NAME})

if(BUILD_TESTING)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
  include(GoogleTest)

  set(PROJECT_NAME_TEST ${PROJECT_NAME}-test)

  add_executable(${PROJECT_NAME_TEST})

  target_sources(${PROJECT_NAME_TEST} 
           PRIVATE test/gtesting.h
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
  )

  target_include_directories(${PROJECT_NAME_TEST} PRIVATE ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/test)
  target_link_libraries(${PROJECT_NAME_TEST} gtest_main gtest ${LIBRARY_NAME})
  target_compile_options(${PROJECT_NAME_TEST} PRIVATE "-g")
  target_link_options(${PROJECT_NAME_TEST} PRIVATE "-lstdc++" PRIVATE "-lm")
  gtest_discover_tests(${PROJECT_NAME_TEST} WORKING_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME_TEST}>)
  enable_testing()

  add_custom_command(
    TARGET ${PROJECT_NAME_TEST}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/test/test_data ${CMAKE_CURRENT_BINARY_DIR}/test_data
    COMMENT "Copy test_data"
  )
endif()
