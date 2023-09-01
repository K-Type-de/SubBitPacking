#include <gtest/gtest.h>
#include <math.h>

#include "../test_sizes.h"
#include "bitpacked.h"

using namespace kt;

template <typename T>
class BitPackedArrayTest : public ::testing::Test
{
};

TYPED_TEST_SUITE(BitPackedArrayTest, test_state_sizes);

// Write/Read Test
TYPED_TEST(BitPackedArrayTest, WriteReadTest)
{
  static constexpr auto num_states = TypeParam::value;
  static constexpr uint32_t values = 100000;

  BitPackedArray<num_states, values> array{};

  for (size_t i = 0; i < values; ++i)
  {
    array.set(i, (i + 1) % num_states);
  }

  for (size_t i = 0; i < values; ++i)
  {
    array.set(i, i % num_states);
  }

  for (size_t i = 0; i < values; ++i)
  {
    EXPECT_EQ(array.get(i), i % num_states) << "Index: " << i;
  }
}

// Write/Read Test (Iterator)
TYPED_TEST(BitPackedArrayTest, WriteReadIteratorTest)
{
  static constexpr auto num_states = TypeParam::value;
  static constexpr uint32_t values = 100000;

  BitPackedArray<num_states, values> array{};

  for (size_t i = 0; i < values; ++i)
  {
    array.set(i, i % num_states);
  }

  size_t count = 0;
  for (PackedState state : array)
  {
    ASSERT_EQ(state, count % num_states) << "Index: " << count;
    count++;
  }
}

// Exception test
TYPED_TEST(BitPackedArrayTest, ExceptionTest)
{
  static constexpr auto num_states = TypeParam::value;
  static constexpr uint32_t values = 1000;
  uint32_t accesses = 2000;

  BitPackedArray<num_states, values> array{};

  for (int i = 0; i < accesses; ++i)
  {
    bool caught = false;
    try
    {
      array.get(i);
    }
    catch (std::out_of_range ex)
    {
      caught = true;
    }

    EXPECT_EQ(caught, i >= values) << "Index: " << i;
  }
}
