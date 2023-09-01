#include <gtest/gtest.h>

#include "../test_sizes.h"
#include "superbitpacked.h"

using namespace kt;

template <typename T>
class SuperBitPackedArrayNoExceptionTest : public ::testing::Test
{
};

TYPED_TEST_SUITE(SuperBitPackedArrayNoExceptionTest, test_state_sizes);

// Tests that no exceptions are thrown
TYPED_TEST(SuperBitPackedArrayNoExceptionTest, NoExceptionTest)
{
  static constexpr auto num_states = TypeParam::value;
  static constexpr uint32_t values = 1000;
  static constexpr uint32_t accesses = values * 2;

  // Avoid undefined behavior when accessing the array out of limits
  uint8_t buf[sizeof(SuperBitPackedArray<num_states, accesses>)];
  auto *array_ptr = new (buf) SuperBitPackedArray<num_states, values>{};

  for (int i = 0; i < accesses; ++i)
  {
    bool caught = false;
    try
    {
      array_ptr->get(i);
    }
    catch (std::out_of_range ex)
    {
      caught = true;
    }

    EXPECT_FALSE(caught) << "Index: " << i;
  }
}
