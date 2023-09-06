#ifndef KT_BITPACKEDARRAY_H
#define KT_BITPACKEDARRAY_H

#include "../base/packedarray.h"
#include "../utils/compiletime.h"
#include "../utils/packedstate.h"

namespace kt
{
template <uint16_t NumStates, std::size_t NumValues>
class BitPackedArray : PackedStateArray<NumStates, NumValues>
{
  static_assert(NumStates > 1 && NumStates < 65536,
                "[BitPackedArray] Number of states must be between 2 and 65535");

private:
  static constexpr uint8_t kBitsPerState = compiletime::NumberOfBitsPerValue(NumStates - 1);
  static constexpr uint16_t kBitMask = compiletime::BitMaskForValue(NumStates - 1);

  static constexpr std::size_t kEntrySize =
      (NumValues * kBitsPerState) / 32 +
      ((NumValues * kBitsPerState % 32 != 0)
           ? 1  // Add +1 array entry if it does not already fit perfectly
           : 0);

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

  BitPackedArray() = default;

  PackedState get(std::size_t value_index) const override
  {
    const std::size_t entry_index = (value_index * kBitsPerState) / 32;
    const std::size_t bit_index = (value_index * kBitsPerState) % 32;

    this->checkAllBoundries(entry_index, kEntrySize, value_index);

    if (bit_index + kBitsPerState <= 32)
    {
      return (this->entries_[entry_index] >> bit_index) & kBitMask;
    }

    return ((this->entries_[entry_index] >> bit_index) |
            (this->entries_[entry_index + 1] << 32 - bit_index)) &
           kBitMask;
  }

  void set(std::size_t value_index, PackedState state) override
  {
    const std::size_t entry_index = (value_index * kBitsPerState) / 32;
    const std::size_t bit_index = (value_index * kBitsPerState) % 32;

    this->checkAllBoundries(entry_index, kEntrySize, value_index);

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

#endif  // KT_BITPACKEDARRAY_H
