#include <gtest/gtest.h>

#include "subbitpacked.h"

using namespace kt;

TEST(SubBitStructTest, NoExceptionTest)
{
  // Tests if exceptions are thrown when accessing states out of range
  SubBitPackedStruct<1, 2, 3, 4> packed_struct;
  size_t num_values = packed_struct.getNumberOfValues();

  for (int i = 0; i < num_values * 2; ++i)
  {
    bool caught = false;
    try
    {
      packed_struct.get(i);
    }
    catch (std::out_of_range)
    {
      caught = true;
    }

    EXPECT_EQ(caught, false);
  }
}
