project(raymann LANGUAGES CXX)
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(RUN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(RUN_TEST_DIR ${PROJECT_SOURCE_DIR}/test/bin)
set(EXE raymann)

set(sources main.cpp src/tools/tools.h)

add_executable(${EXE} ${sources})
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

add_executable(TestTools test/testTools.cpp src/tools/tools.h)
include_directories($ENV{GMOCK_HOME}/include $ENV{GMOCK_HOME}/gtest/include)
target_link_libraries(TestTools gtest_main gtest pthread)
set_target_properties(TestTools PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RUN_TEST_DIR})
add_test(TestTools ${RUN_TEST_DIR}/TestTools)

#include(GoogleTest)
#gtest_discover_tests(hello_test)

