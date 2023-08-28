#include <gtest/gtest.h>
#include <math.h>

#include "../test_sizes.h"
#include "superbitpackedarray.h"


using namespace kt;

template <typename T>
class SuperBitPackedArrayTest : public ::testing::Test
{
};

TYPED_TEST_SUITE(SuperBitPackedArrayTest, test_state_sizes);

// Tests if entries take up correct amount of space (compressed, if possible)
TYPED_TEST(SuperBitPackedArrayTest, SizeTest)
{
  static constexpr auto num_states = TypeParam::value;
  static constexpr size_t num_values = 10000;
  SuperBitPackedArray<num_states, num_values> array{};

  uint8_t number_of_states_per_word = CompileTime::NumberOfStatesPer4ByteWord(num_states);
  std::size_t number_of_entries =
      num_values / number_of_states_per_word + (num_values % number_of_states_per_word != 0);

  uint8_t extra_bits_per_word = CompileTime::NumberOfUnusedUpperBits<uint32_t>(
      CompileTime::Pow<num_states>(number_of_states_per_word) - 1);

  EXPECT_EQ(array.getNumberOfEntries(), number_of_entries);

  if (!extra_bits_per_word)
  {
    // Entries cannot be compressed. Size = number of entries
    EXPECT_EQ(array.getEntrySize(), number_of_entries);
  }
  else
  {
    size_t compressed_size =
        ((number_of_entries * 32) - (number_of_entries * extra_bits_per_word)) / 32 + 1;
    EXPECT_EQ(array.getEntrySize(), compressed_size);
  }
}

// Write/Read Test
TYPED_TEST(SuperBitPackedArrayTest, WriteReadTest)
{
  static constexpr auto num_states = TypeParam::value;
  static constexpr size_t num_values = 10000;
  SuperBitPackedArray<num_states, num_values> array{};

  for (int i = 0; i < num_values; ++i)
  {
    uint16_t set_state = i % num_states;
    array.set(i, set_state);
  }

  for (int i = 0; i < num_values; ++i)
  {
    uint16_t set_state = i % num_states;
    uint16_t state = array.get(i);
    EXPECT_EQ(state, set_state) << "Index: " << i;
  }
}

// Write/Read Iterator Test
TYPED_TEST(SuperBitPackedArrayTest, WriteReadIteratorTest)
{
  static constexpr auto num_states = TypeParam::value;
  static constexpr size_t num_values = 10000;
  SuperBitPackedArray<num_states, num_values> array{};

  for (int i = 0; i < num_values; ++i)
  {
    uint16_t set_state = i % num_states;
    array.set(i, set_state);
  }

  size_t count = 0;
  for (uint16_t state : array)
  {
    uint16_t set_state = count % num_states;
    EXPECT_EQ(state, set_state) << "Index: " << count;
    ++count;
  }
}

// Exception test
TYPED_TEST(SuperBitPackedArrayTest, ExceptionTest)
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

    EXPECT_EQ(caught, i >= values) << "Index: " << i;
  }
}