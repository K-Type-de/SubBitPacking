#ifndef KT_SUPERBITPACKEDARRAY_H
#define KT_SUPERBITPACKEDARRAY_H

#include "../base/packedarray.h"
#include "../utils/compiletime.h"
#include "../utils/packedstate.h"
#include "../utils/subbitpackeddata.h"
#include "../utils/superbitpackedentrybuffer.h"

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

  // Calculate power lookup table at compile time
  static constexpr std::array<uint32_t, kStatesPer4ByteWord> kPowerLookupTable =
      compiletime::GenerateLut<kStatesPer4ByteWord>(compiletime::Pow<NumStates>);

  SuperBitPackedEntryBuffer<kBitsPerEntry, kNumEntries> buffer_{};

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
    this->checkValueBoundries(value_index);

    const std::size_t entry_index = value_index / kStatesPer4ByteWord;

    const uint32_t entry = this->buffer_.get(entry_index);

    return SubBitPackedData::Get(entry, this->kPowerLookupTable[value_index % kStatesPer4ByteWord],
                                 NumStates);
  }

  void set(std::size_t value_index, PackedState state) override
  {
    this->checkValueBoundries(value_index);

    const std::size_t entry_index = value_index / kStatesPer4ByteWord;

    uint32_t entry = this->buffer_.get(entry_index);

    SubBitPackedData::Set(entry, this->kPowerLookupTable[value_index % kStatesPer4ByteWord],
                          NumStates, state % NumStates);

    this->buffer_.set(entry_index, entry);
  }

  inline std::size_t getEntrySize() const override
  {
    return this->buffer_.getArraySize();
  }

  inline std::size_t getByteSize() const override
  {
    return this->buffer_.getByteSize();
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

#include "superbitpackedarrayiterator.h"

}  // namespace kt

#endif  // KT_SUPERBITPACKEDARRAY_H
