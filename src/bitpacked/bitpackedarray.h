#ifndef KT_BITPACKEDARRAY_H
#define KT_BITPACKEDARRAY_H

#include "../base/packedarray.h"
#include "../utils/bitpackedentrybuffer.h"
#include "../utils/compiletime.h"
#include "../utils/packedstate.h"

namespace kt
{
template <uint16_t NumStates, std::size_t NumValues>
class BitPackedArray : PackedStateArray<NumStates, NumValues>
{
  static constexpr uint8_t kBitsPerState = compiletime::NumberOfBitsPerValue(NumStates - 1);
  static constexpr uint16_t kBitMask = compiletime::BitMaskForValue(NumStates - 1);

  BitPackedEntryBuffer<kBitsPerState, NumValues> buffer_{};

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
    this->checkValueBoundries(value_index);
    return static_cast<PackedState>(this->buffer_.get(value_index));
  }

  void set(std::size_t value_index, PackedState state) override
  {
    this->checkValueBoundries(value_index);
    this->buffer_.set(value_index, state);
  }

  std::size_t getEntrySize() const override
  {
    return this->buffer_.getArraySize();
  }

  std::size_t getByteSize() const override
  {
    return this->buffer_.getByteSize();
  }
};

#include "bitpackedarrayiterator.h"

}  // namespace kt

#endif  // KT_BITPACKEDARRAY_H
