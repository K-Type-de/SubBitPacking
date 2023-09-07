#ifndef KT_SUPERBITPACKEDSTRUCTARRAY_H
#define KT_SUPERBITPACKEDSTRUCTARRAY_H

#include <cstdint>

#include "../base/packedarray.h"
#include "../subbitpacked/subbitpackedstruct.h"
#include "../utils/bitpackedentrybuffer.h"
#include "../utils/packedstate.h"

namespace kt
{
template <typename StructEntry, std::size_t NumStructs>
class SuperBitPackedStructArray : public PackedStructArray<NumStructs>
{
  static constexpr uint8_t kBitsPerEntry = StructEntry::GetBitsUsed();
  static constexpr uint8_t kExtraBitsPerWord = 32 - kBitsPerEntry;
  static constexpr uint32_t kEntryBitMask = compiletime::Pow<2>(kBitsPerEntry) - 1;

  BitPackedEntryBuffer<kBitsPerEntry, NumStructs> buffer_{};

public:
  struct Iterator;
  Iterator begin()
  {
    return Iterator(*this, 0);
  }
  Iterator end()
  {
    return Iterator(*this, NumStructs);
  }

  inline StructEntry getEntryCopy(std::size_t entry_index) const
  {
    this->checkValueBoundries(entry_index);
    return {this->buffer_.get(entry_index)};
  }

  inline PackedState get(std::size_t entry_index, std::size_t state_index) const override
  {
    this->checkValueBoundries(entry_index);
    const StructEntry entry = {this->buffer_.get(entry_index)};
    return entry.get(state_index);
  }

  inline void set(std::size_t entry_index, std::size_t state_index, PackedState state) override
  {
    this->checkValueBoundries(entry_index);
    StructEntry entry = {this->buffer_.get(entry_index)};

    entry.set(state_index, state);

    this->buffer_.set(entry_index, entry.getData());
  }

  inline std::size_t getEntrySize() const override
  {
    return this->buffer_.getArraySize();
  }

  inline std::size_t getByteSize() const override
  {
    return this->buffer_.getByteSize();
  }
};

#include "superbitpackedstructarrayiterator.h"

}  // namespace kt

#endif  // KT_SUPERBITPACKEDSTRUCTARRAY_H
