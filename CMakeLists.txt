cmake_minimum_required(VERSION 3.25)
project(raymann VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug) 
project(raymann CXX)
set(RUN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(RUN_TEST_DIR ${PROJECT_SOURCE_DIR}/test/bin)
set(LIBRARY_NAME l${PROJECT_NAME})

include(FetchContent)
FetchContent_Declare(
  math
  GIT_REPOSITORY https://github.com/anazli/math.git
  GIT_TAG main
)
FetchContent_MakeAvailable(math)

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
            src/color/light.h
            src/composite/builder.cpp
            src/composite/builder.h
            src/composite/intersection.h
            src/composite/iterator.cpp
            src/composite/iterator.h
            src/composite/scene_element.cpp
            src/composite/scene_element.h
            src/container/canvas.cpp
            src/container/canvas.h
            src/distribution/utilities.h
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
            src/stochastic/pdf.cpp
            src/stochastic/pdf.h
            src/stochastic/random.cpp
            src/stochastic/random.h
            src/stochastic/stochastic_method.cpp
            src/stochastic/stochastic_method.h
            src/textures/texture.cpp
            src/textures/texture.h
            src/transformations/transformation.cpp
            src/transformations/transformation.h
)

target_include_directories(${LIBRARY_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(${LIBRARY_NAME} PUBLIC math)

add_executable(${PROJECT_NAME} src/main/raymann.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RUN_DIR})
target_compile_options(${PROJECT_NAME} PRIVATE "-g")
target_link_options(${PROJECT_NAME} PRIVATE "-lstdc++" PRIVATE "-lm")
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBRARY_NAME})

add_subdirectory(test)
