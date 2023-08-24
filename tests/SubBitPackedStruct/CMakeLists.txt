cmake_minimum_required(VERSION 3.14)
project(SubBitPackedStructLib)

include(${CMAKE_CURRENT_SOURCE_DIR}/../CMakeLists.txt)

add_executable(
  SubBitPackedStructTest
  subbitpackedstruct_test.cpp
)

target_link_libraries(
  SubBitPackedStructTest
  GTest::gtest_main
)

gtest_discover_tests(SubBitPackedStructTest)
