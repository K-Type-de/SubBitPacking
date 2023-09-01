#include <gtest/gtest.h>

#include "../mock/subbitpackeddata_mock.h"
#include "subbitpacked.h"

using namespace kt;

TEST(SubBitStructArrayTest, NoExceptionTestFetchEntries)
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

        EXPECT_FALSE(caught_state_value_access);
      }
    }
    catch (std::out_of_range)
    {
      caught_array_access = true;
    }
    EXPECT_FALSE(caught_array_access);
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

      EXPECT_FALSE(caught);
    }
  }
}
