#include "subbitpackedstruct.h"

#include <gtest/gtest.h>

using namespace kt;

TEST(SubBitStructTest, SyntaxCheck)
{
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

  SubBitPackedStruct::Field<29> externField{myStruct};  // can we prevent code like this?
  externField = 3;
}