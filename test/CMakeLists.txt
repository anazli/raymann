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
           PRIVATE gtesting.h
                   application_test.cpp
                   boundingBox_test.cpp
                   camera_test.cpp
                   canvas_test.cpp
                   color_test.cpp
                   material_test.cpp
                   primitive_test.cpp
                   properties_test.cpp
                   transformer_test.cpp
                   world_test.cpp
  )

  target_include_directories(${PROJECT_NAME_TEST} PRIVATE ${PROJECT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR})
  target_link_libraries(${PROJECT_NAME_TEST} gtest_main gtest ${LIBRARY_NAME})
  target_compile_options(${PROJECT_NAME_TEST} PRIVATE "-g")
  target_link_options(${PROJECT_NAME_TEST} PRIVATE "-lstdc++" PRIVATE "-lm")
  gtest_discover_tests(${PROJECT_NAME_TEST} WORKING_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME_TEST}>)
  enable_testing()

  add_custom_command(
    TARGET ${PROJECT_NAME_TEST}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/test_data ${CMAKE_CURRENT_BINARY_DIR}/test_data
    COMMENT "Copy test_data"
  )
endif()
