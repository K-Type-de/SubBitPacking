#include <gtest/gtest.h>

#include "subbitpacked.h"

using namespace kt;

TEST(SubBitStructArrayTest, StructArrayTest)
{
  constexpr uint8_t state_num_0 = 3;
  constexpr uint8_t state_num_1 = 5;
  constexpr uint8_t state_num_2 = 7;
  constexpr uint8_t state_num_3 = 9;

  constexpr std::size_t array_size = 10000;
  constexpr std::size_t loop_size = 100000;

  SubBitPackedStructArray<SubBitPackedStruct<state_num_0, state_num_1, state_num_2, state_num_3>,
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

TEST(SubBitStructArrayTest, StructArrayIteratorTest)
{
  constexpr uint8_t state_num_0 = 2;
  constexpr uint8_t state_num_1 = 3;
  constexpr uint8_t state_num_2 = 4;
  constexpr uint8_t state_num_3 = 5;

  constexpr std::size_t array_size = 10000;

  SubBitPackedStructArray<SubBitPackedStruct<state_num_0, state_num_1, state_num_2, state_num_3>,
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
  for (const auto& sub_bit_packed_struct : array)
  {
    uint16_t ret_state_0 = sub_bit_packed_struct.get(0);
    uint16_t ret_state_1 = sub_bit_packed_struct.get(1);
    uint16_t ret_state_2 = sub_bit_packed_struct.get(2);
    uint16_t ret_state_3 = sub_bit_packed_struct.get(3);

    EXPECT_EQ(ret_state_0, index % state_num_0) << "Index: " << index;
    EXPECT_EQ(ret_state_1, index % state_num_1) << "Index: " << index;
    EXPECT_EQ(ret_state_2, index % state_num_2) << "Index: " << index;
    EXPECT_EQ(ret_state_3, index % state_num_3) << "Index: " << index;
    ++index;
  }
}