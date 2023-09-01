#include <gtest/gtest.h>

#include "subbitpacked.h"

using namespace kt;

TEST(SubBitStructTest, NoExceptionTest)
{
  // Tests if exceptions are thrown when accessing states out of range

  // Avoid undefined behavior when acessing state values out of range
  uint8_t buf[sizeof(SubBitPackedStruct<1, 2, 3, 4, 5, 6, 7, 8, 9, 10>)];
  auto packed_struct_ptr = new (buf) SubBitPackedStruct<1, 2, 3, 4>{};

  size_t num_values = packed_struct_ptr->getNumberOfValues();

  for (int i = 0; i < num_values * 2; ++i)
  {
    bool caught = false;
    try
    {
      packed_struct_ptr->get(i);
    }
    catch (std::out_of_range)
    {
      caught = true;
    }

    EXPECT_EQ(caught, false);
  }
}
