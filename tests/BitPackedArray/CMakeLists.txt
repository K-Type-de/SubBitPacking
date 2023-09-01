cmake_minimum_required(VERSION 3.14)
project(BitPackedArrayLib)

include(${CMAKE_CURRENT_SOURCE_DIR}/../CMakeLists.txt)

add_executable(
  BitPackedArrayTest
  bitpackedarray_test.cpp
)

target_link_libraries(
  BitPackedArrayTest
  GTest::gtest_main
)

target_compile_definitions(BitPackedArrayTest PRIVATE KT_ENABLE_EXCEPTIONS)

add_executable(
  BitPackedArrayNoExceptionTest
  bitpackedarraynoexception_test.cpp
)

target_link_libraries(
  BitPackedArrayNoExceptionTest
  GTest::gtest_main
)

target_compile_definitions(BitPackedArrayNoExceptionTest PRIVATE TEST_SIZES_SHORT)

gtest_discover_tests(BitPackedArrayTest)
gtest_discover_tests(BitPackedArrayNoExceptionTest)
