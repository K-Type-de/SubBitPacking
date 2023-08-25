#ifndef _KT_SUBBITPACKESTRUCTARRAY_H_
#define _KT_SUBBITPACKESTRUCTARRAY_H_

#include <cstdint>

#include "subbitpackedstruct.h"

namespace kt
{

template <typename StructEntry, std::size_t num_entries>
class SubBitPackedStructArray
{
  StructEntry entries_[num_entries];

public:
  inline StructEntry &getEntry(std::size_t entry_index)
  {
    return this->entries_[entry_index];
  }

  inline StructEntry getEntryCopy(std::size_t entry_index)
  {
    return this->getEntry(entry_index);
  }

  inline uint16_t getState(std::size_t entry_index, std::size_t state_index)
  {
    return this->getEntry(entry_index).get(state_index);
  }

  inline void setState(std::size_t entry_index, std::size_t state_index, uint16_t state)
  {
    return this->getEntry(entry_index).set(state_index, state);
  }
};

}  // namespace kt

#endif  // _KT_SUBBITPACKESTRUCTARRAY_H_
