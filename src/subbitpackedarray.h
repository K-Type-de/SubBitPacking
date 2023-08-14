#ifndef _KT_SUBBITPACKEDARRAY_H_
#define _KT_SUBBITPACKEDARRAY_H_

#ifndef SUBBITPACKEDARRAY_NO_EXCEPTIONS
#include <stdexcept>
#endif

#include <cstdint>
#include <limits>

#include "compiletimeutils.h"

namespace kt
{
class SubBitPackedArrayEntry
{
private:
  uint32_t data_;

public:
  SubBitPackedArrayEntry() : data_{0} {}

  inline uint16_t get(uint32_t divisor, uint16_t modulo) const
  {
    return this->data_ / divisor % modulo;
  }

  inline void set(uint32_t power, uint16_t modulo, uint16_t state)
  {
    uint8_t current_state = this->get(power, modulo);
    this->data_ = this->data_ - (current_state * power) + (state * power);
  }

  inline uint32_t getData() const
  {
    return this->data_;
  }
};

template <uint16_t num_states, std::size_t num_values>
class SubBitPackedArray
{
  static_assert(num_states > 1 && num_states < 65536,
                "[SubBitPackedArray] Number of states must be between 2 and 65535");

private:
  static constexpr uint8_t kStatesPer4ByteWord = NumberOfStatesPer4ByteWord(num_states);

  static constexpr std::size_t kEntrySize =
      num_values / kStatesPer4ByteWord +
      (num_values % kStatesPer4ByteWord !=
       0);  // Add +1 array entry if it does not already fit perfectly

  // Calculate power lookup table at compile time
  static constexpr std::array<uint32_t, kStatesPer4ByteWord> kPowerLookupTable =
      generate_pow_lut<kStatesPer4ByteWord>(pow<num_states>);

  SubBitPackedArrayEntry entries_[kEntrySize] = {};

public:
#ifndef SUBBITPACKEDARRAY_NO_ITERATOR
  struct Iterator;
  Iterator begin()
  {
    return Iterator(*this, 0);
  }
  Iterator end()
  {
    return Iterator(*this, num_values);
  }
#endif  // SUBBITPACKEDARRAY_NO_ITERATOR

  SubBitPackedArray() {}

  void set(std::size_t value_index, uint16_t state)
  {
#ifndef SUBBITPACKEDARRAY_NO_EXCEPTIONS
    if (value_index >= num_values)
    {
      throw std::out_of_range{"[SubBitPackedArray] Value index out of range"};
    }
#endif  // SUBBITPACKEDARRAY_NO_EXCEPTIONS
    std::size_t entry_index = value_index / kStatesPer4ByteWord;

    this->entries_[entry_index].set(this->kPowerLookupTable[value_index % kStatesPer4ByteWord],
                                    num_states, state % num_states);
  }

  uint16_t get(std::size_t value_index) const
  {
#ifndef SUBBITPACKEDARRAY_NO_EXCEPTIONS
    if (value_index >= num_values)
    {
      throw std::out_of_range{"[SubBitPackedArray] Value index out of range"};
    }
#endif  // SUBBITPACKEDARRAY_NO_EXCEPTIONS

    std::size_t entry_index = value_index / kStatesPer4ByteWord;
    return this->entries_[entry_index].get(
        this->kPowerLookupTable[value_index % kStatesPer4ByteWord], num_states);
  }

  SubBitPackedArrayEntry getEntry(std::size_t entry_index) const
  {
#ifndef SUBBITPACKEDARRAY_NO_EXCEPTIONS
    if (entry_index >= kEntrySize)
    {
      throw std::out_of_range{"[SubBitPackedArray] Entry index out of range"};
    }
#endif  // SUBBITPACKEDARRAY_NO_EXCEPTIONS
    return this->entries_[entry_index];
  }

  std::size_t getEntrySize() const
  {
    return this->kEntrySize;
  }

  std::size_t getByteSize() const
  {
    return this->getEntrySize() * sizeof(SubBitPackedArrayEntry);
  }
};

// Needed for C++11 linkage
template <uint16_t num_states, std::size_t num_values>
constexpr std::array<uint32_t, SubBitPackedArray<num_states, num_values>::kStatesPer4ByteWord>
    SubBitPackedArray<num_states, num_values>::kPowerLookupTable;

#ifndef SUBBITPACKEDARRAY_NO_ITERATOR
#include "subbitpackedarrayiterator.h"
#endif  // SUBBITPACKEDARRAY_NO_ITERATOR

}  // namespace kt

#endif  //_KT_SUBBITPACKEDARRAY_H_
