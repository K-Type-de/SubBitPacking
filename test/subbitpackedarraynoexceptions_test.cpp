#include <gtest/gtest.h>

#define SUBBITPACKEDARRAY_NO_EXCEPTIONS
#define SUBBITPACKEDARRAY_NO_ITERATOR

#include "subbitpackedarray.h"

using namespace kt;

template <typename T>
class SubBitPackedArrayNoExceptionsTest : public ::testing::Test
{
};

// SubBitPackedArray number of states (2-32)
using test_state_sizes =
    ::testing::Types<std::integral_constant<uint8_t, 2>, std::integral_constant<uint8_t, 3>,
                     std::integral_constant<uint8_t, 4>, std::integral_constant<uint8_t, 5>,
                     std::integral_constant<uint8_t, 6>, std::integral_constant<uint8_t, 7>,
                     std::integral_constant<uint8_t, 8>, std::integral_constant<uint8_t, 9>,
                     std::integral_constant<uint8_t, 10>, std::integral_constant<uint8_t, 11>,
                     std::integral_constant<uint8_t, 12>, std::integral_constant<uint8_t, 13>,
                     std::integral_constant<uint8_t, 14>, std::integral_constant<uint8_t, 15>,
                     std::integral_constant<uint8_t, 16>, std::integral_constant<uint8_t, 17>,
                     std::integral_constant<uint8_t, 18>, std::integral_constant<uint8_t, 19>,
                     std::integral_constant<uint8_t, 20>, std::integral_constant<uint8_t, 21>,
                     std::integral_constant<uint8_t, 22>, std::integral_constant<uint8_t, 23>,
                     std::integral_constant<uint8_t, 24>, std::integral_constant<uint8_t, 25>,
                     std::integral_constant<uint8_t, 26>, std::integral_constant<uint8_t, 27>,
                     std::integral_constant<uint8_t, 28>, std::integral_constant<uint8_t, 29>,
                     std::integral_constant<uint8_t, 30>, std::integral_constant<uint8_t, 31>,
                     std::integral_constant<uint8_t, 32>, std::integral_constant<uint16_t, 1025>>;

TYPED_TEST_SUITE(SubBitPackedArrayNoExceptionsTest, test_state_sizes);

// Tests that no exceptions are thrown
TYPED_TEST(SubBitPackedArrayNoExceptionsTest, NoExceptionTest)
{
  static constexpr auto num_states = TypeParam::value;
  static constexpr uint32_t values = 1000;
  uint32_t accesses = 2000;

  SubBitPackedArray<num_states, values> array{};

  for (int i = 0; i < accesses; ++i)
  {
    bool caught = false;
    try
    {
      array.getState(i);
    }
    catch (std::out_of_range ex)
    {
      caught = true;
    }

    EXPECT_EQ(caught, false);
  }
}