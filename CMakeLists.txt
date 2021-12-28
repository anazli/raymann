project(raymann LANGUAGES CXX)
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(RUN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(RUN_TEST_DIR ${PROJECT_SOURCE_DIR}/test/bin)
set(EXE raymann)

set(sources main.cpp src/tools/tools.h)

add_executable(${EXE} ${sources})
target_include_directories(${EXE} PUBLIC ${PROJECT_SOURCE_DIR}/src)
set_target_properties(${EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RUN_DIR})

#include(FetchContent)
#FetchContent_Declare(
#  googletest
#  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
#)
# For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)
enable_testing()

include_directories($ENV{GMOCK_HOME}/include $ENV{GMOCK_HOME}/gtest/include)

set(test1 TestTools)
add_executable(${test1} test/testTools.cpp src/tools/tools.h)
target_include_directories(${test1} PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(${test1} gtest_main gtest pthread)
set_target_properties(${test1} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RUN_TEST_DIR})
add_test(${test1} ${RUN_TEST_DIR}/${test1})

set(test2 TestCanvas)
add_executable(${test2} test/testCanvas.cpp src/tools/tools.h)
target_include_directories(${test2} PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(${test2} gtest_main gtest pthread)
set_target_properties(${test2} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RUN_TEST_DIR})
add_test(${test2} ${RUN_TEST_DIR}/${test2})

#include(GoogleTest)
#gtest_discover_tests(hello_test)

