#ifndef KT_SUBBITPACKEDARRAY_H
#define KT_SUBBITPACKEDARRAY_H

#include "../base/packedarray.h"
#include "../utils/compiletime.h"
#include "../utils/packedstate.h"
#include "../utils/subbitpackeddata.h"

namespace kt
{
template <uint16_t NumStates, std::size_t NumValues>
class SubBitPackedArray : public PackedStateArray<NumStates, NumValues>
{
private:
  static constexpr uint8_t kStatesPer4ByteWord =
      compiletime::NumberOfStatesPer4ByteSubBitPackedWord(NumStates);

  static constexpr std::size_t kEntrySize =
      NumValues / kStatesPer4ByteWord +
      ((NumValues % kStatesPer4ByteWord != 0)
           ? 1  // Add +1 array entry if it does not already fit perfectly
           : 0);

  // Calculate power lookup table at compile time
  static constexpr std::array<uint32_t, kStatesPer4ByteWord> kPowerLookupTable =
      compiletime::GeneratePowLut<kStatesPer4ByteWord>(compiletime::Pow<NumStates>);

  uint32_t entries_[kEntrySize] = {0};

public:
  struct Iterator;
  Iterator begin()
  {
    return Iterator(*this, 0);
  }
  Iterator end()
  {
    return Iterator(*this, NumValues);
  }

  SubBitPackedArray() = default;

  PackedState get(std::size_t value_index) const override
  {
    this->checkValueBoundries(value_index);
    const std::size_t entry_index = value_index / kStatesPer4ByteWord;

    return SubBitPackedData::Get(this->entries_[entry_index],
                                 this->kPowerLookupTable[value_index % kStatesPer4ByteWord],
                                 NumStates);
  }

  void set(std::size_t value_index, PackedState state) override
  {
    this->checkValueBoundries(value_index);
    const std::size_t entry_index = value_index / kStatesPer4ByteWord;

    SubBitPackedData::Set(this->entries_[entry_index],
                          this->kPowerLookupTable[value_index % kStatesPer4ByteWord], NumStates,
                          state % NumStates);
  }

  std::size_t getEntrySize() const override
  {
    return this->kEntrySize;
  }

  std::size_t getByteSize() const override
  {
    return this->getEntrySize() * sizeof(this->entries_[0]);
  }
};

// Needed for C++11 linkage
template <uint16_t NumStates, std::size_t NumValues>
constexpr std::array<uint32_t, SubBitPackedArray<NumStates, NumValues>::kStatesPer4ByteWord>
    SubBitPackedArray<NumStates, NumValues>::kPowerLookupTable;

#include "subbitpackedarrayiterator.h"

}  // namespace kt

#endif  // KT_SUBBITPACKEDARRAY_H
