#include <gtest/gtest.h>

#include "subbitpacked.h"

using namespace kt;

#if defined(__clang__)
const char *Compiler = "CLANG";
#elif defined(__GNUC__) || defined(__GNUG__)
const char *Compiler = "GCC";
#elif defined(_MSC_VER)
const char *Compiler = "MSVC";
#else
const char *Compiler = "OTHER";
#endif

TEST(SubBitStructTest, StructTest)
{
  constexpr uint8_t state_num_0 = 3;
  constexpr uint8_t state_num_1 = 5;
  constexpr uint8_t state_num_2 = 7;
  constexpr uint8_t state_num_3 = 9;

  SubBitPackedStruct<state_num_0, state_num_1, state_num_2, state_num_3> packed_struct;

  for (size_t i = 0; i < 100000; ++i)
  {
    packed_struct.set(0, i % state_num_0);
    packed_struct.set(1, i % state_num_1);
    packed_struct.set(2, i % state_num_2);
    packed_struct.set(3, i % state_num_3);

    PackedState ret_state_0 = packed_struct.get(0);
    PackedState ret_state_1 = packed_struct.get(1);
    PackedState ret_state_2 = packed_struct.get(2);
    PackedState ret_state_3 = packed_struct.get(3);

    EXPECT_EQ(ret_state_0, i % state_num_0) << "Index: " << i;
    EXPECT_EQ(ret_state_1, i % state_num_1) << "Index: " << i;
    EXPECT_EQ(ret_state_2, i % state_num_2) << "Index: " << i;
    EXPECT_EQ(ret_state_3, i % state_num_3) << "Index: " << i;
  }
}

TEST(SubBitStructTest, ConstructorTest)
{
  // Tests if the constructor sets the correct states
  PackedState value_1_0 = 1;
  PackedState value_1_1 = 4;
  PackedState value_1_2 = 5;
  SubBitPackedStruct<2, 7, 8> struct_1{value_1_0, value_1_1, value_1_2};

  EXPECT_EQ(struct_1.get(0), value_1_0);
  EXPECT_EQ(struct_1.get(1), value_1_1);
  EXPECT_EQ(struct_1.get(2), value_1_2);

  PackedState value_2_0 = 1;
  PackedState value_2_1 = 4;
  PackedState value_2_2 = 5;
  SubBitPackedStruct<17, 23, 65, 3> struct_2{value_2_0, value_2_1, value_2_2};

  EXPECT_EQ(struct_2.get(0), value_2_0);
  EXPECT_EQ(struct_2.get(1), value_2_1);
  EXPECT_EQ(struct_2.get(2), value_2_2);
  EXPECT_EQ(struct_2.get(3), 0);

  SubBitPackedStruct<45, 23, 11, 4> struct_3{};

  EXPECT_EQ(struct_3.get(0), 0);
  EXPECT_EQ(struct_3.get(1), 0);
  EXPECT_EQ(struct_3.get(2), 0);
  EXPECT_EQ(struct_3.get(3), 0);
}

TEST(SubBitStructTest, SizeTest)
{
  // Tests if the underlying datatype is optimized for the number of used bits
  SubBitPackedStruct<3, 3, 3, 3> struct_7_bit;
  SubBitPackedStruct<3, 3, 3, 3, 3, 3, 3, 3, 3> struct_15_bit;
  SubBitPackedStruct<3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3> struct_23_bit;
  SubBitPackedStruct<3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3> struct_31_bit;

  EXPECT_EQ(struct_7_bit.GetBitsUsed(), 7) << "Compiler: " << Compiler;
  EXPECT_EQ(struct_7_bit.getDataSize(), 8) << "Compiler: " << Compiler;

  EXPECT_EQ(struct_15_bit.GetBitsUsed(), 15) << "Compiler: " << Compiler;
  EXPECT_EQ(struct_15_bit.getDataSize(), 16) << "Compiler: " << Compiler;

  EXPECT_EQ(struct_23_bit.GetBitsUsed(), 23) << "Compiler: " << Compiler;
  EXPECT_EQ(struct_23_bit.getDataSize(), 24) << "Compiler: " << Compiler;

  EXPECT_EQ(struct_31_bit.GetBitsUsed(), 31) << "Compiler: " << Compiler;
  EXPECT_EQ(struct_31_bit.getDataSize(), 32) << "Compiler: " << Compiler;
}

TEST(SubBitStructTest, ExceptionTest)
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

    EXPECT_EQ(caught, i >= num_values);
  }
}
