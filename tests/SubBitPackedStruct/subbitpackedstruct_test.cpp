#include <gtest/gtest.h>

#include "subbitpacked.h"

using namespace kt;

#if defined(__GNUC__)
const char *Compiler = "GNU";
#elif defined(_MSC_VER)
const char *Compiler = "MSVC";
#else
const char *Compiler = "OTHER";
#endif

#include <iostream>

TEST(SubBitStructTest, StructTest)
{
  constexpr uint8_t state_num_0 = 3;
  constexpr uint8_t state_num_1 = 5;
  constexpr uint8_t state_num_2 = 7;
  constexpr uint8_t state_num_3 = 9;

  SubBitPackedStruct<state_num_0, state_num_1, state_num_2, state_num_3> tstruct;

  size_t a = tstruct.GetBitsUsed();
  size_t b = tstruct.getDataSize();

  SubBitPackedStruct<3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3> teststruct;
  EXPECT_EQ(teststruct.GetBitsUsed(), 23);
  EXPECT_EQ(teststruct.getDataSize(), 24) << "Compiler: " << Compiler;

  size_t c = teststruct.GetBitsUsed();
  size_t d = teststruct.getDataSize();

  for (size_t i = 0; i < 100000; ++i)
  {
    tstruct.set(0, i % state_num_0);
    tstruct.set(1, i % state_num_1);
    tstruct.set(2, i % state_num_2);
    tstruct.set(3, i % state_num_3);

    uint16_t ret_state_0 = tstruct.get(0);
    uint16_t ret_state_1 = tstruct.get(1);
    uint16_t ret_state_2 = tstruct.get(2);
    uint16_t ret_state_3 = tstruct.get(3);

    EXPECT_EQ(ret_state_0, i % state_num_0) << "Index: " << i;
    EXPECT_EQ(ret_state_1, i % state_num_1) << "Index: " << i;
    EXPECT_EQ(ret_state_2, i % state_num_2) << "Index: " << i;
    EXPECT_EQ(ret_state_3, i % state_num_3) << "Index: " << i;
  }
}
