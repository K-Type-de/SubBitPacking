#ifndef _KT_SUBBITPACKEDSTRUCTARRAY_H_
#define _KT_SUBBITPACKEDSTRUCTARRAY_H_

#include <cstdint>

#include "../base/packedarray.h"
#include "subbitpackedstruct.h"

namespace kt
{

template <typename StructEntry, std::size_t num_entries>
class SubBitPackedStructArray : public PackedStructArray<num_entries>
{
  StructEntry entries_[num_entries];

public:
  struct Iterator;
  Iterator begin()
  {
    return Iterator(*this, 0);
  }
  Iterator end()
  {
    return Iterator(*this, num_entries);
  }

  inline StructEntry &getEntry(std::size_t entry_index)
  {
    this->checkValueBoundries(entry_index);
    return this->entries_[entry_index];
  }

  inline StructEntry getEntryCopy(std::size_t entry_index) const
  {
    return this->getEntry(entry_index);
  }

  inline uint16_t get(std::size_t entry_index, std::size_t state_index) const override
  {
    this->checkValueBoundries(entry_index);
    return const_cast<SubBitPackedStructArray *>(this)->getEntry(entry_index).get(state_index);
  }

  inline void set(std::size_t entry_index, std::size_t state_index, uint16_t state) override
  {
    this->checkValueBoundries(entry_index);
    return this->getEntry(entry_index).set(state_index, state);
  }

  std::size_t getEntrySize() const override
  {
    return num_entries;
  }

  std::size_t getByteSize() const override
  {
    return num_entries * sizeof(StructEntry);
  }
};

#include "subbitpackedstructarrayiterator.h"

}  // namespace kt

#endif  // _KT_SUBBITPACKEDSTRUCTARRAY_H_
