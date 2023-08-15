cmake_minimum_required(VERSION 3.14)
project(SubBitPackedArrayLib)

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

include_directories(../src)

enable_testing()

add_executable(
  SubBitPackedTest
  subbitpackedarray_test.cpp
  subbitpackedstruct_test.cpp
)

target_link_libraries(
  SubBitPackedTest
  GTest::gtest_main
)

add_executable(
  SubBitPackedArrayNoExceptionsTest
  subbitpackedarraynoexceptions_test.cpp
)

target_link_libraries(
  SubBitPackedArrayNoExceptionsTest
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(SubBitPackedTest)
gtest_discover_tests(SubBitPackedArrayNoExceptionsTest)
