#include <gtest/gtest.h>

#include "superbitpackedstructarray.h"

using namespace kt;

TEST(SuperBitStructArrayTest, StructArrayTest)
{
  constexpr uint8_t state_num_0 = 3;
  constexpr uint8_t state_num_1 = 5;
  constexpr uint8_t state_num_2 = 7;
  constexpr uint8_t state_num_3 = 9;

  constexpr std::size_t array_size = 10000;
  constexpr std::size_t loop_size = 100000;

  SuperBitPackedStructArray<SubBitPackedStruct<state_num_0, state_num_1, state_num_2, state_num_3>,
                            array_size>
      array;

  for (size_t i = 0; i < loop_size; ++i)
  {
    array.setState(i % array_size, 0, i % state_num_0);
    array.setState(i % array_size, 1, i % state_num_1);
    array.setState(i % array_size, 2, i % state_num_2);
    array.setState(i % array_size, 3, i % state_num_3);

    uint16_t ret_state_0 = array.getState(i % array_size, 0);
    uint16_t ret_state_1 = array.getState(i % array_size, 1);
    uint16_t ret_state_2 = array.getState(i % array_size, 2);
    uint16_t ret_state_3 = array.getState(i % array_size, 3);

    EXPECT_EQ(ret_state_0, i % state_num_0) << "Index: " << i;
    EXPECT_EQ(ret_state_1, i % state_num_1) << "Index: " << i;
    EXPECT_EQ(ret_state_2, i % state_num_2) << "Index: " << i;
    EXPECT_EQ(ret_state_3, i % state_num_3) << "Index: " << i;
  }
}