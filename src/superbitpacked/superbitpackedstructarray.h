#ifndef KT_SUPERBITPACKEDSTRUCTARRAY_H
#define KT_SUPERBITPACKEDSTRUCTARRAY_H

#include <cstdint>

#include "../base/packedarray.h"
#include "../subbitpacked/subbitpackedstruct.h"
#include "../utils/packedstate.h"
#include "../utils/superbitpackedarrayentrymetadata.h"

namespace kt
{
template <typename StructEntry, std::size_t NumStructs>
class SuperBitPackedStructArray : public PackedStructArray<NumStructs>
{
  static constexpr uint8_t kBitsPerEntry = StructEntry::GetBitsUsed();
  static constexpr uint8_t kExtraBitsPerWord = 32 - kBitsPerEntry;

  // 8, 16, 24 and 32-bit can be stored in SubBitPackedStructArray without wasting bits
  static_assert(kBitsPerEntry % 8 != 0,
                "[SuperBitPackedStructArray] Cannot SuperBitPack structs with no extra bits. Use "
                "\"SubBitPackedStructArray\" instead");

  static constexpr std::size_t kEntrySize =
      (NumStructs * kBitsPerEntry) / 32 +
      (((NumStructs * kBitsPerEntry) % 32 != 0)
           ? 1  // Add +1 array entry if it does not already fit perfectly
           : 0);

  uint32_t storage_[kEntrySize];

  inline internal::SuperBitPackedArrayEntryMetadata getEntryMetadata(std::size_t entry_index) const
  {
    const std::size_t bit_address = (entry_index * 32) - (entry_index * kExtraBitsPerWord);
    const std::size_t start_index = bit_address / 32;
    const std::size_t bit_shift = bit_address % 32;

    return {start_index, bit_shift};
  }

  inline StructEntry getEntryInternal(std::size_t start_index, std::size_t bit_shift) const
  {
    if (!bit_shift)
    {
      uint32_t entry = this->storage_[start_index];
      // Shift out upper bits
      entry <<= kExtraBitsPerWord;
      entry >>= kExtraBitsPerWord;
      return static_cast<StructEntry>(entry);
    }

    const uint32_t &entry_1 = this->storage_[start_index];
    const uint32_t &entry_2 = this->storage_[start_index + 1];

    uint32_t combined_entry = (entry_1 >> bit_shift) | (entry_2 << (32 - bit_shift));
    // Shift out upper bits
    combined_entry <<= kExtraBitsPerWord;
    combined_entry >>= kExtraBitsPerWord;
    return static_cast<StructEntry>(combined_entry);
  }

  inline void setEntryInternal(std::size_t start_index, std::size_t bit_shift, StructEntry entry)
  {
    if (!bit_shift)
    {
      // Clear old bits
      this->storage_[start_index] >>= kBitsPerEntry;
      this->storage_[start_index] <<= kBitsPerEntry;
      // Store new value
      this->storage_[start_index] |= entry.getData();
      return;
    }

    // Clear old bits
    this->storage_[start_index] <<= 32 - bit_shift;
    this->storage_[start_index] >>= 32 - bit_shift;
    this->storage_[start_index + 1] >>= bit_shift - 1;
    this->storage_[start_index + 1] <<= bit_shift - 1;

    // Store new value
    this->storage_[start_index] |= entry.getData() << bit_shift;
    this->storage_[start_index + 1] |= entry.getData() >> (32 - bit_shift);
  }

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
    auto metadata = this->getEntryMetadata(entry_index);
    return this->getEntryInternal(metadata.start_index, metadata.bit_shift);
  }

  inline PackedState get(std::size_t entry_index, std::size_t state_index) const override
  {
    this->checkValueBoundries(entry_index);
    auto metadata = this->getEntryMetadata(entry_index);
    const StructEntry entry = this->getEntryInternal(metadata.start_index, metadata.bit_shift);

    return entry.get(state_index);
  }

  inline void set(std::size_t entry_index, std::size_t state_index, PackedState state) override
  {
    this->checkValueBoundries(entry_index);
    auto metadata = this->getEntryMetadata(entry_index);
    StructEntry entry = this->getEntryInternal(metadata.start_index, metadata.bit_shift);

    entry.set(state_index, state);

    this->setEntryInternal(metadata.start_index, metadata.bit_shift, entry);
  }

  std::size_t getEntrySize() const override
  {
    return kEntrySize;
  }

  std::size_t getByteSize() const override
  {
    return kEntrySize * sizeof(uint32_t);
  }
};

#include "superbitpackedstructarrayiterator.h"

}  // namespace kt

#endif  // KT_SUPERBITPACKEDSTRUCTARRAY_H
