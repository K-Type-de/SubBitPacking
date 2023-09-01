#include <gtest/gtest.h>

#include "subbitpacked.h"

using namespace kt;

TEST(SubBitStructArrayTest, NoExceptionTestFetchEntries)
{
  constexpr std::size_t array_size = 100;
  constexpr std::size_t loop_size = array_size * 2;

  SubBitPackedStructArray<SubBitPackedStruct<1, 2, 3, 4>, array_size> array;
  size_t num_state_values = array.getEntry(0).getNumberOfValues();

  for (size_t i = 0; i < loop_size; ++i)
  {
    bool caught_array_access = false;

    try
    {
      auto entry = array.getEntry(i);

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

  SubBitPackedStructArray<SubBitPackedStruct<1, 2, 3, 4>, array_size> array;
  size_t num_state_values = array.getEntry(0).getNumberOfValues();

  for (size_t i = 0; i < loop_size; ++i)
  {
    bool caught = false;

    for (size_t j = 0; j < num_state_values * 2; ++j)
    {
      try
      {
        array.get(i, j);
      }
      catch (std::out_of_range)
      {
        caught = true;
      }

      EXPECT_FALSE(caught);
    }
  }
}
