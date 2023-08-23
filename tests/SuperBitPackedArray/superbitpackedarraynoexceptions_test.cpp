#include <gtest/gtest.h>

#include "../test_sizes.h"
#include "superbitpackedarray.h"

using namespace kt;

template <typename T>
class SuperBitPackedArrayNoExceptionsTest : public ::testing::Test
{
};

TYPED_TEST_SUITE(SuperBitPackedArrayNoExceptionsTest, test_state_sizes);

// Tests that no exceptions are thrown
TYPED_TEST(SuperBitPackedArrayNoExceptionsTest, NoExceptionTest)
{
  static constexpr auto num_states = TypeParam::value;
  static constexpr uint32_t values = 1000;
  uint32_t accesses = 2000;

  SuperBitPackedArray<num_states, values> array{};

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

    EXPECT_EQ(caught, false) << "Index: " << i;
  }
}