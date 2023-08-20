#include "subbitpackedstruct.h"

#include <gtest/gtest.h>

using namespace kt;

TEST(SubBitStructTest, SyntaxCheck)
{

  TStruct<3,5,7,9> tstruct;

  EXPECT_EQ(4, tstruct.kNumFields);


  EXPECT_EQ(3, tstruct.kNumStates[0]);
  EXPECT_EQ(5, tstruct.kNumStates[1]);
  EXPECT_EQ(7, tstruct.kNumStates[2]);
  EXPECT_EQ(9, tstruct.kNumStates[3]);


  EXPECT_EQ(1, tstruct.kStatePowers[0]);
  EXPECT_EQ(3, tstruct.kStatePowers[1]);
  EXPECT_EQ(15, tstruct.kStatePowers[2]);
  EXPECT_EQ(105, tstruct.kStatePowers[3]);


  class MyStruct : public SubBitPackedStruct
  {
  public:
    Field<3> state1{*this};
    Field<7> state2{*this};
    Field<29> state3{*this};
  };

  MyStruct myStruct;

  myStruct.state1 = 1;

  uint32_t readstate = myStruct.state1;
  myStruct.state1 = myStruct.state3;


}