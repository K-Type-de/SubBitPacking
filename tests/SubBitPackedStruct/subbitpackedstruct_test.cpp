#include <gtest/gtest.h>

#include "subbitpackedstruct.h"

using namespace kt;

// TEST(SubBitStructTest, SyntaxCheck)
// {
//   TStruct<3, 5, 7, 9> tstruct;

//   EXPECT_EQ(4, tstruct.kNumFields);

//   EXPECT_EQ(3, tstruct.kNumStates[0]);
//   EXPECT_EQ(5, tstruct.kNumStates[1]);
//   EXPECT_EQ(7, tstruct.kNumStates[2]);
//   EXPECT_EQ(9, tstruct.kNumStates[3]);

//   EXPECT_EQ(1, tstruct.kStatePowers[0]);
//   EXPECT_EQ(3, tstruct.kStatePowers[1]);
//   EXPECT_EQ(15, tstruct.kStatePowers[2]);
//   EXPECT_EQ(105, tstruct.kStatePowers[3]);

//   class MyStruct : public SubBitPackedStruct
//   {
//   public:
//     Field<3> state1{*this};
//     Field<7> state2{*this};
//     Field<29> state3{*this};
//   };

//   MyStruct myStruct;

//   myStruct.state1 = 1;

//   uint32_t readstate = myStruct.state1;
//   myStruct.state1 = myStruct.state3;
// }

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
  EXPECT_EQ(teststruct.getDataSize(), 24);

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