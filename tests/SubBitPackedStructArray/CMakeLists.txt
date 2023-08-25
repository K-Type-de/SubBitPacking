cmake_minimum_required(VERSION 3.14)
project(SubBitPackedStructArrayLib)

include(${CMAKE_CURRENT_SOURCE_DIR}/../CMakeLists.txt)

add_executable(
  SubBitPackedStructArrayTest
  subbitpackedstructarray_test.cpp
)

target_link_libraries(
  SubBitPackedStructArrayTest
  GTest::gtest_main
)

gtest_discover_tests(SubBitPackedStructArrayTest)
