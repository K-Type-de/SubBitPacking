#ifndef _KT_SUPERBITPACKEDARRAY_H_
#define _KT_SUPERBITPACKEDARRAY_H_

#include "../base/packedarray.h"
#include "../utils/compiletime.h"
#include "../utils/subbitpackeddata.h"
#include "../utils/superbitpackedarrayentrymetadata.h"

namespace kt
{

template <uint16_t num_states, std::size_t num_values>
class SuperBitPackedArray : PackedArray<num_states, num_values>
{
public:
  static constexpr uint8_t kStatesPer4ByteWord =
      CompileTime::NumberOfStatesPer4ByteSubBitPackedWord(num_states);

  static constexpr uint8_t kExtraBitsPerWord = CompileTime::NumberOfUnusedUpperBits<uint32_t>(
      CompileTime::Pow<num_states>(kStatesPer4ByteWord) - 1);

  static constexpr std::size_t kNumEntries =
      num_values / kStatesPer4ByteWord +
      (num_values % kStatesPer4ByteWord !=
       0);  // Add +1 array entry if it does not already fit perfectly

  static constexpr std::size_t kEntrySize =
      kExtraBitsPerWord == 0 ? kNumEntries
                             : ((kNumEntries * 32) - (kNumEntries * kExtraBitsPerWord)) / 32 + 1;

  // Calculate power lookup table at compile time
  static constexpr std::array<uint32_t, kStatesPer4ByteWord> kPowerLookupTable =
      CompileTime::GeneratePowLut<kStatesPer4ByteWord>(CompileTime::Pow<num_states>);

  uint32_t entries_[kEntrySize] = {0};

  template <int N = kExtraBitsPerWord, typename std::enable_if<(N > 0), int>::type = 0>
  inline Internal::SuperBitPackedArrayEntryMetadata getEntryMetadata(std::size_t value_index) const
  {
    this->checkValueBoundries(value_index);
    const std::size_t entry_index = value_index / kStatesPer4ByteWord;
    const std::size_t bit_address = (entry_index * 32) - (entry_index * kExtraBitsPerWord);
    const std::size_t start_index = bit_address / 32;
    const std::size_t bit_shift = bit_address % 32;

    return {start_index, bit_shift};
  }

  template <int N = kExtraBitsPerWord, typename std::enable_if<(N > 0), int>::type = 0>
  inline uint32_t getEntry(std::size_t start_index, std::size_t bit_shift) const
  {
    if (!bit_shift)
    {
      uint32_t entry = this->entries_[start_index];
      // Shift out upper bits
      entry <<= kExtraBitsPerWord;
      entry >>= kExtraBitsPerWord;
      return entry;
    }

    const uint32_t &entry_1 = this->entries_[start_index];
    const uint32_t &entry_2 = this->entries_[start_index + 1];

    uint32_t combined_entry = (entry_1 >> bit_shift) | (entry_2 << (32 - bit_shift));
    // Shift out upper bits
    combined_entry <<= kExtraBitsPerWord;
    combined_entry >>= kExtraBitsPerWord;
    return combined_entry;
  }

  template <int N = kExtraBitsPerWord, typename std::enable_if<(N > 0), int>::type = 0>
  inline void setEntry(std::size_t start_index, std::size_t bit_shift, uint32_t entry)
  {
    if (!bit_shift)
    {
      // Clear old bits
      this->entries_[start_index] >>= 32 - kExtraBitsPerWord;
      this->entries_[start_index] <<= 32 - kExtraBitsPerWord;
      // Store new value
      this->entries_[start_index] |= entry;
      return;
    }

    // Clear old bits
    this->entries_[start_index] <<= 32 - bit_shift;
    this->entries_[start_index] >>= 32 - bit_shift;
    this->entries_[start_index + 1] >>= bit_shift - 1;
    this->entries_[start_index + 1] <<= bit_shift - 1;

    // Store new value
    this->entries_[start_index] |= entry << bit_shift;
    this->entries_[start_index + 1] |= entry >> (32 - bit_shift);
  }

  /**
   * get/set for num_states where there is no extra bit(s) available for compression.
   * Works like SubBitPackedArray
   */
  template <int N = kExtraBitsPerWord, typename std::enable_if<(N == 0), int>::type = 0>
  inline uint16_t _get(std::size_t value_index) const
  {
    this->checkValueBoundries(value_index);
    const std::size_t entry_index = value_index / kStatesPer4ByteWord;

    return SubBitPackedData::Get(this->entries_[entry_index],
                                 this->kPowerLookupTable[value_index % kStatesPer4ByteWord],
                                 num_states);
  }

  template <int N = kExtraBitsPerWord, typename std::enable_if<(N == 0), int>::type = 0>
  inline void _set(std::size_t value_index, uint16_t state)
  {
    this->checkValueBoundries(value_index);
    const std::size_t entry_index = value_index / kStatesPer4ByteWord;

    SubBitPackedData::Set(this->entries_[entry_index],
                          this->kPowerLookupTable[value_index % kStatesPer4ByteWord], num_states,
                          state % num_states);
  }
  /********************************************************************************/

  /**
   * get/set for num_states where extra bit(s) available for compression.
   */
  template <int N = kExtraBitsPerWord, typename std::enable_if<(N > 0), int>::type = 0>
  inline uint16_t _get(std::size_t value_index) const
  {
    auto metadata = this->getEntryMetadata(value_index);
    const uint32_t entry = this->getEntry(metadata.start_index, metadata.bit_shift);

    return SubBitPackedData::Get(entry, this->kPowerLookupTable[value_index % kStatesPer4ByteWord],
                                 num_states);
  }

  template <int N = kExtraBitsPerWord, typename std::enable_if<(N > 0), int>::type = 0>
  inline void _set(std::size_t value_index, uint16_t state)
  {
    auto metadata = this->getEntryMetadata(value_index);
    uint32_t entry = this->getEntry(metadata.start_index, metadata.bit_shift);

    SubBitPackedData::Set(entry, this->kPowerLookupTable[value_index % kStatesPer4ByteWord],
                          num_states, state % num_states);

    this->setEntry(metadata.start_index, metadata.bit_shift, entry);
  }
  /********************************************************************************/

public:
  SuperBitPackedArray() {}

  struct Iterator;
  Iterator begin()
  {
    return Iterator(*this, 0);
  }
  Iterator end()
  {
    return Iterator(*this, num_values);
  }

  uint16_t get(std::size_t value_index) const override
  {
    return this->_get(value_index);
  }

  void set(std::size_t value_index, uint16_t state) override
  {
    this->_set(value_index, state);
  }

  inline std::size_t getEntrySize() const override
  {
    return this->kEntrySize;
  }

  inline std::size_t getByteSize() const override
  {
    return this->getEntrySize() * sizeof(this->entries_[0]);
  }

  inline std::size_t getNumberOfEntries() const
  {
    return this->kNumEntries;
  }
};

// Needed for C++11 linkage
template <uint16_t num_states, std::size_t num_values>
constexpr std::array<uint32_t, SuperBitPackedArray<num_states, num_values>::kStatesPer4ByteWord>
    SuperBitPackedArray<num_states, num_values>::kPowerLookupTable;

template <uint16_t num_states, std::size_t num_values>
constexpr uint8_t SuperBitPackedArray<num_states, num_values>::kExtraBitsPerWord;

template <uint16_t num_states, std::size_t num_values>
constexpr std::size_t SuperBitPackedArray<num_states, num_values>::kNumEntries;

template <uint16_t num_states, std::size_t num_values>
constexpr std::size_t SuperBitPackedArray<num_states, num_values>::kEntrySize;

#include "superbitpackedarrayiterator.h"

}  // namespace kt

#endif  //_KT_SUPERBITPACKEDARRAY_H_
