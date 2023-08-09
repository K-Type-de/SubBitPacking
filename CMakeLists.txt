cmake_minimum_required(VERSION 3.14)
project(SubBitPackedArrayLib)

include_directories(src)

if(DEFINED ENV{RUN_UNIT_TESTS})
  message(STATUS "Building Unit Tests")

  # GoogleTest requires at least C++14
  set(CMAKE_CXX_STANDARD 14)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)

  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  )
  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)

  enable_testing()

  add_executable(
    SubBitPackedArrayTest
    test/subbitpackedarray_test.cpp
    src/subbitpackedarray.cpp
    src/subbitpackedarray.h
  )

  target_link_libraries(
    SubBitPackedArrayTest
    GTest::gtest_main
  )

  add_executable(
    SubBitPackedArrayNoExceptionsTest
    test/subbitpackedarraynoexceptions_test.cpp
    src/subbitpackedarray.cpp
    src/subbitpackedarray.h
  )

  target_link_libraries(
    SubBitPackedArrayNoExceptionsTest
    GTest::gtest_main
  )

  include(GoogleTest)
  gtest_discover_tests(SubBitPackedArrayTest)
  gtest_discover_tests(SubBitPackedArrayNoExceptionsTest)

else()

message(STATUS "Building Library")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_library(SubBitPackedArray src/subbitpackedarray.cpp src/subbitpackedarray.h src/subbitpackedarrayiterator.h src/compiletimeutils.h)

endif()