cmake_minimum_required(VERSION 3.14)
project(SubBitPackedStructLib)

include(${CMAKE_CURRENT_SOURCE_DIR}/../CMakeLists.txt)

add_executable(
  SubBitPackedStruct
  subbitpackedstruct_test.cpp
)

target_link_libraries(
  SubBitPackedStruct
  GTest::gtest_main
)

gtest_discover_tests(SubBitPackedStruct)

