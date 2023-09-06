#ifndef KT_SUBBITPACKEDSTRUCTARRAY_H
#define KT_SUBBITPACKEDSTRUCTARRAY_H

#include <cstdint>

#include "../base/packedarray.h"
#include "../utils/packedstate.h"
#include "subbitpackedstruct.h"

namespace kt
{

template <typename StructEntry, std::size_t NumEntries>
class SubBitPackedStructArray : public PackedStructArray<NumEntries>
{
  StructEntry entries_[NumEntries];

public:
  struct Iterator;
  Iterator begin()
  {
    return Iterator(*this, 0);
  }
  Iterator end()
  {
    return Iterator(*this, NumEntries);
  }

  inline StructEntry &getEntry(std::size_t entry_index)
  {
    this->checkArrayBoundries(entry_index, NumEntries);
    return this->entries_[entry_index];
  }

  inline StructEntry getEntryCopy(std::size_t entry_index) const
  {
    return this->getEntry(entry_index);
  }

  inline PackedState get(std::size_t entry_index, std::size_t state_index) const override
  {
    this->checkAllBoundries(entry_index, NumEntries, entry_index);
    return const_cast<SubBitPackedStructArray *>(this)->getEntry(entry_index).get(state_index);
  }

  inline void set(std::size_t entry_index, std::size_t state_index, PackedState state) override
  {
    this->checkAllBoundries(entry_index, NumEntries, entry_index);
    return this->getEntry(entry_index).set(state_index, state);
  }

  std::size_t getEntrySize() const override
  {
    return NumEntries;
  }

  std::size_t getByteSize() const override
  {
    return NumEntries * sizeof(StructEntry);
  }
};

#include "subbitpackedstructarrayiterator.h"

}  // namespace kt

#endif  // KT_SUBBITPACKEDSTRUCTARRAY_H
