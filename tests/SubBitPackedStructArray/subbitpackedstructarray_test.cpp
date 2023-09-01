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

    PackedState ret_state_0 = array.get(i % array_size, 0);
    PackedState ret_state_1 = array.get(i % array_size, 1);
    PackedState ret_state_2 = array.get(i % array_size, 2);
    PackedState ret_state_3 = array.get(i % array_size, 3);

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
  for (const auto &sub_bit_packed_struct : array)
  {
    PackedState ret_state_0 = sub_bit_packed_struct.get(0);
    PackedState ret_state_1 = sub_bit_packed_struct.get(1);
    PackedState ret_state_2 = sub_bit_packed_struct.get(2);
    PackedState ret_state_3 = sub_bit_packed_struct.get(3);

    EXPECT_EQ(ret_state_0, index % state_num_0) << "Index: " << index;
    EXPECT_EQ(ret_state_1, index % state_num_1) << "Index: " << index;
    EXPECT_EQ(ret_state_2, index % state_num_2) << "Index: " << index;
    EXPECT_EQ(ret_state_3, index % state_num_3) << "Index: " << index;
    ++index;
  }
}

TEST(SubBitStructArrayTest, ExceptionTestFetchEntries)
{
  constexpr std::size_t array_size = 10000;
  constexpr std::size_t loop_size = array_size * 2;

  // Avoid undefined behavior when acessing array entries or state values out of range
  uint8_t buf[sizeof(
      SubBitPackedStructArray<SubBitPackedStruct<1, 2, 3, 4, 5, 6, 7, 8, 9, 10>, loop_size>)];
  auto *array_ptr =
      new (buf) SubBitPackedStructArray<SubBitPackedStruct<1, 2, 3, 4>, array_size>{};

  size_t num_state_values = array_ptr->getEntry(0).getNumberOfValues();

  for (size_t i = 0; i < loop_size; ++i)
  {
    bool caught_array_access = false;

    try
    {
      auto entry = array_ptr->getEntry(i);

      for (size_t j = 0; j < num_state_values * 2; ++j)
      {
        bool caught_state_value_access = false;
        try
        {
          entry.get(j);
        }
        catch (std::out_of_range)
        {
          caught_state_value_access = true;
        }

        EXPECT_EQ(caught_state_value_access, j >= num_state_values);
      }
    }
    catch (std::out_of_range)
    {
      caught_array_access = true;
    }

    EXPECT_EQ(caught_array_access, i >= array_size);
  }
}

TEST(SubBitStructArrayTest, ExceptionTestDirectAccess)
{
  constexpr std::size_t array_size = 10000;
  constexpr std::size_t loop_size = array_size * 2;

  // Avoid undefined behavior when acessing array entries or state values out of range
  uint8_t buf[sizeof(
      SubBitPackedStructArray<SubBitPackedStruct<1, 2, 3, 4, 5, 6, 7, 8, 9, 10>, loop_size>)];
  auto *array_ptr =
      new (buf) SubBitPackedStructArray<SubBitPackedStruct<1, 2, 3, 4>, array_size>{};

  size_t num_state_values = array_ptr->getEntry(0).getNumberOfValues();

  for (size_t i = 0; i < loop_size; ++i)
  {
    bool caught = false;

    for (size_t j = 0; j < num_state_values * 2; ++j)
    {
      try
      {
        array_ptr->get(i, j);
      }
      catch (std::out_of_range)
      {
        caught = true;
      }

      EXPECT_EQ(caught, i >= array_size || j >= num_state_values);
    }
  }
}
