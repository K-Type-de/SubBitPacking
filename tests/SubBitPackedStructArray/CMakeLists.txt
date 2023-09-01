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

target_compile_definitions(SubBitPackedStructArrayTest PRIVATE KT_ENABLE_EXCEPTIONS)

add_executable(
  SubBitPackedStructArrayNoExceptionTest
  subbitpackedstructarraynoexception_test.cpp
)

target_link_libraries(
  SubBitPackedStructArrayNoExceptionTest
  GTest::gtest_main
)

gtest_discover_tests(SubBitPackedStructArrayTest)
gtest_discover_tests(SubBitPackedStructArrayNoExceptionTest)
