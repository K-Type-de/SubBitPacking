#include <gtest/gtest.h>
#include <math.h>

#include "subbitpackedarray.h"

using namespace kt;

template <typename T>
class SubBitPackedArrayTest : public ::testing::Test
{
};

// SubBitPackedArray number of states (2 through 32 + selected other values)
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
                     std::integral_constant<uint8_t, 32>, std::integral_constant<uint8_t, 33>,
                     std::integral_constant<uint8_t, 34>, std::integral_constant<uint8_t, 35>,
                     std::integral_constant<uint8_t, 36>, std::integral_constant<uint8_t, 37>,
                     std::integral_constant<uint8_t, 38>, std::integral_constant<uint8_t, 39>,
                     std::integral_constant<uint8_t, 40>, std::integral_constant<uint8_t, 65>,
                     std::integral_constant<uint8_t, 66>, std::integral_constant<uint8_t, 67>,
                     std::integral_constant<uint8_t, 68>, std::integral_constant<uint8_t, 69>,
                     std::integral_constant<uint8_t, 70>, std::integral_constant<uint8_t, 71>,
                     std::integral_constant<uint8_t, 72>, std::integral_constant<uint8_t, 73>,
                     std::integral_constant<uint8_t, 74>, std::integral_constant<uint8_t, 75>,
                     std::integral_constant<uint8_t, 76>, std::integral_constant<uint8_t, 77>,
                     std::integral_constant<uint8_t, 78>, std::integral_constant<uint8_t, 79>,
                     std::integral_constant<uint8_t, 80>, std::integral_constant<uint8_t, 81>,
                     std::integral_constant<uint8_t, 82>, std::integral_constant<uint8_t, 83>,
                     std::integral_constant<uint8_t, 84>, std::integral_constant<uint16_t, 1025>,
                     std::integral_constant<uint16_t, 65535>>;

TYPED_TEST_SUITE(SubBitPackedArrayTest, test_state_sizes);

// Tests if precalculated NumberOfStatesPer4ByteWord returns correct values
TYPED_TEST(SubBitPackedArrayTest, NumberOfStatesPerEntryTest)
{
  static constexpr auto num_states = TypeParam::value;
  uint8_t states_per_4_byte_word = NumberOfStatesPer4ByteWord(num_states);
  uint8_t states_per_4_byte_word_calculated = sizeof(uint32_t) * 8 / (log(num_states) / log(2));

  EXPECT_EQ(states_per_4_byte_word, states_per_4_byte_word_calculated);
}

// Tests that a SubBitPackedArray can hold the correct amount of states in a single entry,
// everything above (+1) generates another entry
TYPED_TEST(SubBitPackedArrayTest, MinEntrySizeTest)
{
  static constexpr auto num_states = TypeParam::value;
  static constexpr uint8_t states_per_4_byte_word = NumberOfStatesPer4ByteWord(num_states);

  SubBitPackedArray<num_states, states_per_4_byte_word> one_entry_array{};
  EXPECT_EQ(one_entry_array.getEntrySize(), 1);

  SubBitPackedArray<num_states, states_per_4_byte_word + 1> two_entry_array{};
  EXPECT_EQ(two_entry_array.getEntrySize(), 2);
}

// Test Entry/Byte sizes
TYPED_TEST(SubBitPackedArrayTest, SizeTest)
{
  static constexpr auto num_states = TypeParam::value;
  static const uint8_t states_per_4_byte_word = sizeof(uint32_t) * 8 / (log(num_states) / log(2));
  static constexpr uint32_t values = 100000;

  size_t expected_number_of_entries = values / states_per_4_byte_word;
  if (values % states_per_4_byte_word != 0)
  {
    ++expected_number_of_entries;
  }
  size_t expected_number_of_bytes = 4 * expected_number_of_entries;

  SubBitPackedArray<num_states, values> array{};

  EXPECT_EQ(array.getEntrySize(), expected_number_of_entries);
  EXPECT_EQ(array.getByteSize(), expected_number_of_bytes);
}

// Write/Read Test
TYPED_TEST(SubBitPackedArrayTest, WriteReadTest)
{
  static constexpr auto num_states = TypeParam::value;
  static constexpr uint32_t values = 100000;

  SubBitPackedArray<num_states, values> array{};

  for (size_t i = 0; i < values; ++i)
  {
    array.set(i, i % num_states);
  }

  for (size_t i = 0; i < values; ++i)
  {
    ASSERT_EQ(array.get(i), i % num_states);
  }
}

// Write/Read Test (Iterator)
TYPED_TEST(SubBitPackedArrayTest, WriteReadIteratorTest)
{
  static constexpr auto num_states = TypeParam::value;
  static constexpr uint32_t values = 100000;

  SubBitPackedArray<num_states, values> array{};

  for (size_t i = 0; i < values; ++i)
  {
    array.set(i, i % num_states);
  }

  size_t count = 0;
  for (uint16_t state : array)
  {
    ASSERT_EQ(state, count++ % num_states);
  }
}

// Exception test
TYPED_TEST(SubBitPackedArrayTest, ExceptionTest)
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
      array.get(i);
    }
    catch (std::out_of_range ex)
    {
      caught = true;
    }

    EXPECT_EQ(caught, i >= values);
  }
}