cmake_minimum_required(VERSION 3.14)
project(SuperBitPackedStructArrayLib)

include(${CMAKE_CURRENT_SOURCE_DIR}/../CMakeLists.txt)

add_executable(
  SuperBitPackedStructArrayTest
  superbitpackedstructarray_test.cpp
)

target_link_libraries(
  SuperBitPackedStructArrayTest
  GTest::gtest_main
)

target_compile_definitions(SuperBitPackedStructArrayTest PRIVATE KT_ENABLE_EXCEPTIONS)

gtest_discover_tests(SuperBitPackedStructArrayTest)
