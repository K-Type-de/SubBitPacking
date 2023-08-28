#include <gtest/gtest.h>
#include <math.h>

#include "../test_sizes.h"
#include "subbitpackedarray.h"

using namespace kt;

template <typename T>
class SubBitPackedArrayTest : public ::testing::Test
{
};

TYPED_TEST_SUITE(SubBitPackedArrayTest, test_state_sizes);

// Tests if precalculated NumberOfStatesPer4ByteSubBitPackedWord returns correct values
TYPED_TEST(SubBitPackedArrayTest, NumberOfStatesPerEntryTest)
{
  static constexpr auto num_states = TypeParam::value;
  uint8_t states_per_4_byte_word = CompileTime::NumberOfStatesPer4ByteSubBitPackedWord(num_states);
  uint8_t states_per_4_byte_word_calculated = sizeof(uint32_t) * 8 / (log(num_states) / log(2));

  EXPECT_EQ(states_per_4_byte_word, states_per_4_byte_word_calculated);
}

// Tests that a SubBitPackedArray can hold the correct amount of states in a single entry,
// everything above (+1) generates another entry
TYPED_TEST(SubBitPackedArrayTest, MinEntrySizeTest)
{
  static constexpr auto num_states = TypeParam::value;
  static constexpr uint8_t states_per_4_byte_word =
      CompileTime::NumberOfStatesPer4ByteSubBitPackedWord(num_states);

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
    ASSERT_EQ(array.get(i), i % num_states) << "Index: " << i;
    ;
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
    ASSERT_EQ(state, count % num_states) << "Index: " << count;
    count++;
  }
}

// Iterator random start test
TYPED_TEST(SubBitPackedArrayTest, IteratorRandTest)
{
  // Tests correct iterator implementation if the iterator does not start at the beginning
  static constexpr auto num_states = TypeParam::value;
  static constexpr uint32_t values = 100;
  uint32_t start_index = 51;

  SubBitPackedArray<num_states, values> array{};

  for (size_t i = 0; i < values; ++i)
  {
    array.set(i, i % num_states);
  }

  typename SubBitPackedArray<num_states, values>::Iterator iterator{array, start_index};

  while (iterator != array.end())
  {
    uint16_t state = *iterator;

    ASSERT_EQ(state, start_index % num_states);
    ASSERT_EQ(state, array.get(start_index));
    start_index++;
    iterator++;
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

    EXPECT_EQ(caught, i >= values) << "Index: " << i;
  }
}