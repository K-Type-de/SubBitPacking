#ifndef KT_SUPERBITPACKEDENTRYBUFFER_H
#define KT_SUPERBITPACKEDENTRYBUFFER_H

#include <cstdint>

#include "../base/packedarray.h"
#include "compiletime.h"

namespace kt
{

namespace internal
{
struct BitPackedArrayEntryMetadata
{
  std::size_t start_index;
  std::size_t bit_shift;
};
}  // namespace internal

template <uint8_t BitsPerEntry, std::size_t NumberOfEntries>
class BitPackedEntryBuffer : private PackedArray<NumberOfEntries>
{
  static constexpr uint8_t kExtraBitsPerEntry = 32 - BitsPerEntry;
  static constexpr uint32_t kEntryBitMask = compiletime::Pow<2>(BitsPerEntry) - 1;

  static constexpr std::size_t kArraySize =
      (BitsPerEntry * NumberOfEntries) / 32 + ((BitsPerEntry * NumberOfEntries) % 32 == 0 ? 0 : 1);

  uint32_t storage_[kArraySize] = {0};

  template <int N = kExtraBitsPerEntry, typename std::enable_if<(N > 0), int>::type = 0>
  inline internal::BitPackedArrayEntryMetadata getEntryMetadata(std::size_t entry_index) const
  {
    const std::size_t bit_address = (entry_index * 32) - (entry_index * kExtraBitsPerEntry);
    const std::size_t start_index = bit_address / 32;
    const std::size_t bit_shift = bit_address % 32;

    this->checkArrayBoundries(start_index + (bit_shift + BitsPerEntry > 32 ? 1 : 0), kArraySize);
    return {start_index, bit_shift};
  }

  template <int N = kExtraBitsPerEntry, typename std::enable_if<(N > 0), int>::type = 0>
  inline uint32_t getEntry(const internal::BitPackedArrayEntryMetadata &metadata) const
  {
    if (metadata.bit_shift + BitsPerEntry > 32)
    {
      const uint32_t &entry_1 = this->storage_[metadata.start_index];
      const uint32_t &entry_2 = this->storage_[metadata.start_index + 1];

      uint32_t combined_entry =
          ((entry_1 >> metadata.bit_shift) | (entry_2 << (32 - metadata.bit_shift))) &
          kEntryBitMask;
      return combined_entry;
    }

    const uint32_t entry_value =
        ((this->storage_[metadata.start_index] & (kEntryBitMask << metadata.bit_shift)) >>
         metadata.bit_shift);
    return entry_value;
  }

  template <int N = kExtraBitsPerEntry, typename std::enable_if<(N > 0), int>::type = 0>
  inline void setEntry(const internal::BitPackedArrayEntryMetadata &metadata, uint32_t entry)
  {
    if (metadata.bit_shift + BitsPerEntry > 32)
    {
      const uint8_t bits_in_1st_entry = 32 - metadata.bit_shift;
      const uint8_t bits_in_2nd_entry = BitsPerEntry - bits_in_1st_entry;

      // Clear old bits
      this->storage_[metadata.start_index] &=
          ~((kEntryBitMask >> bits_in_2nd_entry) << 32 - bits_in_1st_entry);
      this->storage_[metadata.start_index + 1] &= ~(kEntryBitMask >> bits_in_1st_entry);

      // Store new value
      this->storage_[metadata.start_index] |= ((entry << bits_in_2nd_entry) & kEntryBitMask)
                                              << 32 - BitsPerEntry;
      this->storage_[metadata.start_index + 1] |= entry >> bits_in_1st_entry;

      return;
    }

    this->storage_[metadata.start_index] &= ~(kEntryBitMask << metadata.bit_shift);
    this->storage_[metadata.start_index] |= entry << metadata.bit_shift;
  };

public:
  BitPackedEntryBuffer() = default;

  template <int N = kExtraBitsPerEntry, typename std::enable_if<(N == 0), int>::type = 0>
  inline uint32_t get(std::size_t entry_index) const
  {
    return this->storage_[entry_index];
  }
  template <int N = kExtraBitsPerEntry, typename std::enable_if<(N == 0), int>::type = 0>
  inline void set(std::size_t entry_index, uint32_t entry)
  {
    this->storage_[entry_index] = entry;
  }

  template <int N = kExtraBitsPerEntry, typename std::enable_if<(N > 0), int>::type = 0>
  inline uint32_t get(std::size_t entry_index) const
  {
    auto metadata = this->getEntryMetadata(entry_index);
    return this->getEntry(metadata);
  }

  template <int N = kExtraBitsPerEntry, typename std::enable_if<(N > 0), int>::type = 0>
  inline void set(std::size_t entry_index, uint32_t entry)
  {
    auto metadata = this->getEntryMetadata(entry_index);
    return this->setEntry(metadata, entry);
  }

  std::size_t getArraySize() const
  {
    return kArraySize;
  }

  std::size_t getByteSize() const
  {
    return kArraySize * sizeof(uint32_t);
  }
};

}  // namespace kt

#endif  // KT_SUPERBITPACKEDENTRYBUFFER_H
