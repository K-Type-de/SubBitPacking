cmake_minimum_required(VERSION 3.14)
project(SubBitPackedArrayLib)

include(${CMAKE_CURRENT_SOURCE_DIR}/../CMakeLists.txt)

add_executable(
  SubBitPackedArrayTest
  subbitpackedarray_test.cpp
)

target_link_libraries(
  SubBitPackedArrayTest
  GTest::gtest_main
)

target_compile_definitions(SubBitPackedArrayTest PRIVATE KT_ENABLE_EXCEPTIONS)

add_executable(
  SubBitPackedArrayNoExceptionsTest
  subbitpackedarraynoexceptions_test.cpp
)

target_link_libraries(
  SubBitPackedArrayNoExceptionsTest
  GTest::gtest_main
)

target_compile_definitions(SubBitPackedArrayNoExceptionsTest PRIVATE TEST_SIZES_SHORT)

gtest_discover_tests(SubBitPackedArrayTest)
gtest_discover_tests(SubBitPackedArrayNoExceptionsTest)

