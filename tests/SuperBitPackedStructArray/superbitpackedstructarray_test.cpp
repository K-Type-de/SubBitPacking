#include <gtest/gtest.h>

#include <bitset>

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
    PackedState ret_state_0 = array.get(i, 0);
    PackedState ret_state_1 = array.get(i, 1);
    PackedState ret_state_2 = array.get(i, 2);
    PackedState ret_state_3 = array.get(i, 3);

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
  for (const auto &super_bit_packed_struct : array)
  {
    PackedState ret_state_0 = super_bit_packed_struct.get(0);
    PackedState ret_state_1 = super_bit_packed_struct.get(1);
    PackedState ret_state_2 = super_bit_packed_struct.get(2);
    PackedState ret_state_3 = super_bit_packed_struct.get(3);

    EXPECT_EQ(ret_state_0, index % state_num_0) << "Index: " << index;
    EXPECT_EQ(ret_state_1, index % state_num_1) << "Index: " << index;
    EXPECT_EQ(ret_state_2, index % state_num_2) << "Index: " << index;
    EXPECT_EQ(ret_state_3, index % state_num_3) << "Index: " << index;
    ++index;
  }
}

TEST(SuperBitStructArrayTest, ExceptionTestFetchEntries)
{
  constexpr std::size_t array_size = 10000;
  constexpr std::size_t loop_size = array_size * 2;

  // Avoid undefined behavior when acessing array entries or state values out of range
  uint8_t buf[sizeof(
      SuperBitPackedStructArray<SubBitPackedStruct<1, 2, 3, 4, 5, 6, 7, 8, 9, 10>, loop_size>)];
  auto *array_ptr =
      new (buf) SuperBitPackedStructArray<SubBitPackedStruct<1, 2, 3, 4>, array_size>{};

  size_t num_state_values = array_ptr->getEntryCopy(0).getNumberOfValues();

  for (size_t i = 0; i < loop_size; ++i)
  {
    bool caught_array_access = false;

    try
    {
      auto entry = array_ptr->getEntryCopy(i);

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

TEST(SuperBitStructArrayTest, ExceptionTestDirectAccess)
{
  constexpr std::size_t array_size = 10000;
  constexpr std::size_t loop_size = array_size * 2;

  // Avoid undefined behavior when acessing array entries or state values out of range
  uint8_t buf[sizeof(
      SuperBitPackedStructArray<SubBitPackedStruct<1, 2, 3, 4, 5, 6, 7, 8, 9, 10>, loop_size>)];
  auto *array_ptr =
      new (buf) SuperBitPackedStructArray<SubBitPackedStruct<1, 2, 3, 4>, array_size>{};

  size_t num_state_values = array_ptr->getEntryCopy(0).getNumberOfValues();

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
