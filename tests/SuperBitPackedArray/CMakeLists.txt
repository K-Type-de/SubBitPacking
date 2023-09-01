cmake_minimum_required(VERSION 3.14)
project(SuperBitPackedArrayLib)

include(${CMAKE_CURRENT_SOURCE_DIR}/../CMakeLists.txt)

add_executable(
  SuperBitPackedArrayTest
  superbitpackedarray_test.cpp
)

target_link_libraries(
  SuperBitPackedArrayTest
  GTest::gtest_main
)

target_compile_definitions(SuperBitPackedArrayTest PRIVATE KT_ENABLE_EXCEPTIONS)

add_executable(
  SuperBitPackedArrayNoExceptionTest
  superbitpackedarraynoexception_test.cpp
)

target_link_libraries(
  SuperBitPackedArrayNoExceptionTest
  GTest::gtest_main
)

target_compile_definitions(SuperBitPackedArrayNoExceptionTest PRIVATE TEST_SIZES_SHORT)

gtest_discover_tests(SuperBitPackedArrayTest)
gtest_discover_tests(SuperBitPackedArrayNoExceptionTest)
