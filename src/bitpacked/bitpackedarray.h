#ifndef _KT_BITPACKEDARRAY_H_
#define _KT_BITPACKEDARRAY_H_

#include "../base/packedarray.h"
#include "../utils/compiletime.h"

namespace kt
{
template <uint16_t num_states, std::size_t num_values>
class BitPackedArray : PackedStateArray<num_states, num_values>
{
  static_assert(num_states > 1 && num_states < 65536,
                "[BitPackedArray] Number of states must be between 2 and 65535");

private:
  static constexpr uint8_t kBitsPerState = CompileTime::NumberOfBitsPerValue(num_states - 1);
  static constexpr uint16_t kBitMask = CompileTime::BitMaskForValue(num_states - 1);

  static constexpr std::size_t kEntrySize =
      (num_values * kBitsPerState) / 32 +
      ((num_values * kBitsPerState) % 32 !=
       0);  // Add +1 array entry if it does not already fit perfectly

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

  BitPackedArray() {}

  uint16_t get(std::size_t value_index) const override
  {
    this->checkValueBoundries(value_index);
    const std::size_t entry_index = (value_index * kBitsPerState) / 32;
    const std::size_t bit_index = (value_index * kBitsPerState) % 32;

    if (bit_index + kBitsPerState <= 32)
    {
      return (this->entries_[entry_index] >> bit_index) & kBitMask;
    }

    return ((this->entries_[entry_index] >> bit_index) |
            (this->entries_[entry_index + 1] << 32 - bit_index)) &
           kBitMask;
  }

  void set(std::size_t value_index, uint16_t state) override
  {
    this->checkValueBoundries(value_index);
    const std::size_t entry_index = (value_index * kBitsPerState) / 32;
    const std::size_t bit_index = (value_index * kBitsPerState) % 32;

    if (bit_index + kBitsPerState <= 32)
    {
      // Clear old value
      this->entries_[entry_index] &= ~(kBitMask << bit_index);

      // Set new value
      this->entries_[entry_index] |= (state & kBitMask) << bit_index;

      return;
    }

    // Clear old values
    this->entries_[entry_index] &= ~(kBitMask << bit_index);
    this->entries_[entry_index + 1] &= ~(kBitMask >> (32 - bit_index));

    // Set new values
    this->entries_[entry_index] |= state << bit_index;
    this->entries_[entry_index + 1] |= (state >> (32 - bit_index));

    return;
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

#include "bitpackedarrayiterator.h"

}  // namespace kt

#endif  //_KT_BITPACKEDARRAY_H_
