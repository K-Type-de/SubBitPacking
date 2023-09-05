#ifndef KT_SUPERBITPACKEDARRAY_H
#define KT_SUPERBITPACKEDARRAY_H

#include "../base/packedarray.h"
#include "../utils/compiletime.h"
#include "../utils/packedstate.h"
#include "../utils/subbitpackeddata.h"
#include "../utils/superbitpackedarrayentrymetadata.h"

namespace kt
{

template <uint16_t NumStates, std::size_t NumValues>
class SuperBitPackedArray : public PackedStateArray<NumStates, NumValues>
{
private:
  static constexpr uint8_t kStatesPer4ByteWord =
      compiletime::NumberOfStatesPer4ByteSubBitPackedWord(NumStates);
  static constexpr uint8_t kExtraBitsPerWord = compiletime::NumberOfUnusedUpperBits<uint32_t>(
      compiletime::Pow<NumStates>(kStatesPer4ByteWord) - 1);
  static constexpr uint8_t kBitsPerEntry = 32 - kExtraBitsPerWord;
  static constexpr uint32_t kEntryBitMask = compiletime::Pow<2>(kBitsPerEntry) - 1;

  static constexpr std::size_t kNumEntries =
      NumValues / kStatesPer4ByteWord +
      ((NumValues % kStatesPer4ByteWord != 0)
           ? 1  // Add +1 array entry if it does not already fit perfectly
           : 0);

  static constexpr std::size_t kEntrySize =
      kExtraBitsPerWord == 0 ? kNumEntries
                             : ((kNumEntries * 32) - (kNumEntries * kExtraBitsPerWord)) / 32 + 1;

  // Calculate power lookup table at compile time
  static constexpr std::array<uint32_t, kStatesPer4ByteWord> kPowerLookupTable =
      compiletime::GeneratePowLut<kStatesPer4ByteWord>(compiletime::Pow<NumStates>);

  uint32_t entries_[kEntrySize] = {0};

  template <int N = kExtraBitsPerWord, typename std::enable_if<(N > 0), int>::type = 0>
  inline internal::SuperBitPackedArrayEntryMetadata getEntryMetadata(std::size_t value_index) const
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
    if (bit_shift + kBitsPerEntry > 32)
    {
      const uint32_t &entry_1 = this->entries_[start_index];
      const uint32_t &entry_2 = this->entries_[start_index + 1];

      uint32_t combined_entry =
          ((entry_1 >> bit_shift) | (entry_2 << (32 - bit_shift))) & kEntryBitMask;
      return combined_entry;
    }

    const uint32_t entry_value =
        ((this->entries_[start_index] & (kEntryBitMask << bit_shift)) >> bit_shift);
    return entry_value;
  }

  template <int N = kExtraBitsPerWord, typename std::enable_if<(N > 0), int>::type = 0>
  inline void setEntry(std::size_t start_index, std::size_t bit_shift, uint32_t entry)
  {
    if (bit_shift + kBitsPerEntry > 32)
    {
      const uint8_t bits_in_1st_entry = 32 - bit_shift;
      const uint8_t bits_in_2nd_entry = kBitsPerEntry - bits_in_1st_entry;

      // Clear old bits
      this->entries_[start_index] &=
          ~((kEntryBitMask >> bits_in_2nd_entry) << 32 - bits_in_1st_entry);
      this->entries_[start_index + 1] &= ~(kEntryBitMask >> bits_in_1st_entry);

      // Store new value
      this->entries_[start_index] |= ((entry << bits_in_2nd_entry) & kEntryBitMask)
                                     << 32 - bits_in_1st_entry - bits_in_2nd_entry;
      this->entries_[start_index + 1] |= entry >> bits_in_1st_entry;

      return;
    }

    this->entries_[start_index] &= ~(kEntryBitMask << bit_shift);
    this->entries_[start_index] |= entry << bit_shift;
  }

  /**
   * get/set for NumStates where there is no extra bit(s) available for compression.
   * Works like SubBitPackedArray
   */
  template <int N = kExtraBitsPerWord, typename std::enable_if<(N == 0), int>::type = 0>
  inline PackedState getInternal(std::size_t value_index) const
  {
    this->checkValueBoundries(value_index);
    const std::size_t entry_index = value_index / kStatesPer4ByteWord;

    return SubBitPackedData::Get(this->entries_[entry_index],
                                 this->kPowerLookupTable[value_index % kStatesPer4ByteWord],
                                 NumStates);
  }

  template <int N = kExtraBitsPerWord, typename std::enable_if<(N == 0), int>::type = 0>
  inline void setInternal(std::size_t value_index, PackedState state)
  {
    this->checkValueBoundries(value_index);
    const std::size_t entry_index = value_index / kStatesPer4ByteWord;

    SubBitPackedData::Set(this->entries_[entry_index],
                          this->kPowerLookupTable[value_index % kStatesPer4ByteWord], NumStates,
                          state % NumStates);
  }
  /********************************************************************************/

  /**
   * get/set for NumStates where extra bit(s) available for compression.
   */
  template <int N = kExtraBitsPerWord, typename std::enable_if<(N > 0), int>::type = 0>
  inline PackedState getInternal(std::size_t value_index) const
  {
    auto metadata = this->getEntryMetadata(value_index);
    const uint32_t entry = this->getEntry(metadata.start_index, metadata.bit_shift);

    return SubBitPackedData::Get(entry, this->kPowerLookupTable[value_index % kStatesPer4ByteWord],
                                 NumStates);
  }

  template <int N = kExtraBitsPerWord, typename std::enable_if<(N > 0), int>::type = 0>
  inline void setInternal(std::size_t value_index, PackedState state)
  {
    auto metadata = this->getEntryMetadata(value_index);
    uint32_t entry = this->getEntry(metadata.start_index, metadata.bit_shift);

    SubBitPackedData::Set(entry, this->kPowerLookupTable[value_index % kStatesPer4ByteWord],
                          NumStates, state % NumStates);

    this->setEntry(metadata.start_index, metadata.bit_shift, entry);
  }
  /********************************************************************************/

public:
  SuperBitPackedArray() = default;

  struct Iterator;
  Iterator begin()
  {
    return Iterator(*this, 0);
  }
  Iterator end()
  {
    return Iterator(*this, NumValues);
  }

  PackedState get(std::size_t value_index) const override
  {
    return this->getInternal(value_index);
  }

  void set(std::size_t value_index, PackedState state) override
  {
    this->setInternal(value_index, state);
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
template <uint16_t NumStates, std::size_t NumValues>
constexpr std::array<uint32_t, SuperBitPackedArray<NumStates, NumValues>::kStatesPer4ByteWord>
    SuperBitPackedArray<NumStates, NumValues>::kPowerLookupTable;

template <uint16_t NumStates, std::size_t NumValues>
constexpr uint8_t SuperBitPackedArray<NumStates, NumValues>::kExtraBitsPerWord;

template <uint16_t NumStates, std::size_t NumValues>
constexpr std::size_t SuperBitPackedArray<NumStates, NumValues>::kNumEntries;

template <uint16_t NumStates, std::size_t NumValues>
constexpr std::size_t SuperBitPackedArray<NumStates, NumValues>::kEntrySize;

#include "superbitpackedarrayiterator.h"

}  // namespace kt

#endif  // KT_SUPERBITPACKEDARRAY_H
