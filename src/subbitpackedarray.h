#ifndef _KT_SUBBITPACKEDARRAY_H_
#define _KT_SUBBITPACKEDARRAY_H_

#include "packedarray.h"
#include "utils/compiletime.h"
#include "utils/subbitpackeddata.h"

namespace kt
{
template <uint16_t num_states, std::size_t num_values>
class SubBitPackedArray : PackedArray<num_states, num_values>
{
private:
  static constexpr uint8_t kStatesPer4ByteWord =
      CompileTime::NumberOfStatesPer4ByteWord(num_states);

  static constexpr std::size_t kEntrySize =
      num_values / kStatesPer4ByteWord +
      (num_values % kStatesPer4ByteWord !=
       0);  // Add +1 array entry if it does not already fit perfectly

  // Calculate power lookup table at compile time
  static constexpr std::array<uint32_t, kStatesPer4ByteWord> kPowerLookupTable =
      CompileTime::GeneratePowLut<kStatesPer4ByteWord>(CompileTime::Pow<num_states>);

  uint32_t entries_[kEntrySize] = {0};

public:
  struct Iterator;
  Iterator begin()
  {
    return Iterator(*this, 0);
  }
  Iterator end()
  {
    return Iterator(*this, num_values);
  }

  SubBitPackedArray() {}

  uint16_t get(std::size_t value_index) const override
  {
    this->checkValueBoundries(value_index);
    const std::size_t entry_index = value_index / kStatesPer4ByteWord;

    return SubBitPackedData::Get(this->entries_[entry_index],
                                 this->kPowerLookupTable[value_index % kStatesPer4ByteWord],
                                 num_states);
  }

  void set(std::size_t value_index, uint16_t state) override
  {
    this->checkValueBoundries(value_index);
    const std::size_t entry_index = value_index / kStatesPer4ByteWord;

    SubBitPackedData::Set(this->entries_[entry_index],
                          this->kPowerLookupTable[value_index % kStatesPer4ByteWord], num_states,
                          state % num_states);
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
template <uint16_t num_states, std::size_t num_values>
constexpr std::array<uint32_t, SubBitPackedArray<num_states, num_values>::kStatesPer4ByteWord>
    SubBitPackedArray<num_states, num_values>::kPowerLookupTable;

#include "subbitpackedarrayiterator.h"

}  // namespace kt

#endif  //_KT_SUBBITPACKEDARRAY_H_
