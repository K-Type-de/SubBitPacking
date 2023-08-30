#include <gtest/gtest.h>

#include "superbitpacked.h"

using namespace kt;

TEST(SuperBitStructArrayTest, StructArrayTest)
{
  constexpr uint8_t state_num_0 = 3;
  constexpr uint8_t state_num_1 = 5;
  constexpr uint8_t state_num_2 = 7;
  constexpr uint8_t state_num_3 = 9;

  constexpr std::size_t array_size = 10023;  // Some uneven number
  constexpr std::size_t loop_size = 100000;

  SuperBitPackedStructArray<SubBitPackedStruct<state_num_0, state_num_1, state_num_2, state_num_3>,
                            array_size>
      array;

  for (size_t i = 0; i < loop_size; ++i)
  {
    array.set(i % array_size, 0, i % state_num_0);
    array.set(i % array_size, 1, i % state_num_1);
    array.set(i % array_size, 2, i % state_num_2);
    array.set(i % array_size, 3, i % state_num_3);

    uint16_t ret_state_0 = array.get(i % array_size, 0);
    uint16_t ret_state_1 = array.get(i % array_size, 1);
    uint16_t ret_state_2 = array.get(i % array_size, 2);
    uint16_t ret_state_3 = array.get(i % array_size, 3);

    EXPECT_EQ(ret_state_0, i % state_num_0) << "Index: " << i;
    EXPECT_EQ(ret_state_1, i % state_num_1) << "Index: " << i;
    EXPECT_EQ(ret_state_2, i % state_num_2) << "Index: " << i;
    EXPECT_EQ(ret_state_3, i % state_num_3) << "Index: " << i;
  }
}

TEST(SuperBitStructArrayTest, StructArrayTest2)
{
  constexpr uint8_t state_num_0 = 2;
  constexpr uint8_t state_num_1 = 4;
  constexpr uint8_t state_num_2 = 8;
  constexpr uint8_t state_num_3 = 12;

  constexpr std::size_t array_size = 9897;  // Some uneven number

  SuperBitPackedStructArray<SubBitPackedStruct<state_num_0, state_num_1, state_num_2, state_num_3>,
                            array_size>
      array;

  for (size_t i = 0; i < array_size; ++i)
  {
    array.set(i, 0, i % state_num_0);
    array.set(i, 1, i % state_num_1);
    array.set(i, 2, i % state_num_2);
    array.set(i, 3, i % state_num_3);
  }

  for (size_t i = 0; i < array_size; ++i)
  {
    uint16_t ret_state_0 = array.get(i, 0);
    uint16_t ret_state_1 = array.get(i, 1);
    uint16_t ret_state_2 = array.get(i, 2);
    uint16_t ret_state_3 = array.get(i, 3);

    EXPECT_EQ(ret_state_0, i % state_num_0) << "Index: " << i;
    EXPECT_EQ(ret_state_1, i % state_num_1) << "Index: " << i;
    EXPECT_EQ(ret_state_2, i % state_num_2) << "Index: " << i;
    EXPECT_EQ(ret_state_3, i % state_num_3) << "Index: " << i;
  }
}

TEST(SuperBitStructArrayTest, StructArrayIteratorTest)
{
  constexpr uint8_t state_num_0 = 2;
  constexpr uint8_t state_num_1 = 6;
  constexpr uint8_t state_num_2 = 3;
  constexpr uint8_t state_num_3 = 9;

  constexpr std::size_t array_size = 12371;  // Some uneven number

  SuperBitPackedStructArray<SubBitPackedStruct<state_num_0, state_num_1, state_num_2, state_num_3>,
                            array_size>
      array;

  for (size_t i = 0; i < array_size; ++i)
  {
    array.set(i, 0, i % state_num_0);
    array.set(i, 1, i % state_num_1);
    array.set(i, 2, i % state_num_2);
    array.set(i, 3, i % state_num_3);
  }

  size_t index = 0;
  for (const auto& super_bit_packed_struct : array)
  {
    uint16_t ret_state_0 = super_bit_packed_struct.get(0);
    uint16_t ret_state_1 = super_bit_packed_struct.get(1);
    uint16_t ret_state_2 = super_bit_packed_struct.get(2);
    uint16_t ret_state_3 = super_bit_packed_struct.get(3);

    EXPECT_EQ(ret_state_0, index % state_num_0) << "Index: " << index;
    EXPECT_EQ(ret_state_1, index % state_num_1) << "Index: " << index;
    EXPECT_EQ(ret_state_2, index % state_num_2) << "Index: " << index;
    EXPECT_EQ(ret_state_3, index % state_num_3) << "Index: " << index;
    ++index;
  }
}
